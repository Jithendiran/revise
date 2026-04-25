## GNOME Development Environment: RAM-Based Workflow

This documentation outlines a professional development setup for contributing to GNOME projects (specifically `gnome-system-monitor`) using **Flatpak**, **Meson**, and **RAM-disks (`tmpfs`)**.

---

### Core Concept: Why use this setup?

1.  **SSD Protection:** Compilation creates thousands of temporary files. By directing these writes to RAM, hardware wear on the SSD is eliminated.
2.  **Speed:** RAM is significantly faster than any SSD, leading to quicker build times.
3.  **Isolation:** Using Flatpak ensures the development environment is identical for all developers, preventing "it works on my machine" errors without cluttering the main operating system with libraries.

---

### Phase 1: Creating the High-Speed Workspace
Every time the computer reboots, the RAM disk must be recreated because RAM is volatile memory.

| Step | Command | Explanation |
| :--- | :--- | :--- |
| **1** | `mkdir -p /tmp/build-tmp` | Creates a folder to serve as the entry point for the RAM disk. |
| **2** | `sudo mount -t tmpfs -o size=12G tmpfs /tmp/build-tmp` | Allocates 12GB of System RAM to behave like a physical disk drive. |
| **3** | `sudo chown $USER:$USER /tmp/build-tmp` | Changes ownership so the user can write files without `sudo`. |

---

### Phase 2: Building the Sandbox (Dependencies)
The project requires specific versions of libraries (GTK4, GLibmm, Libadwaita). These are defined in the `org.ji.test.json` manifest.

```bash
flatpak-builder --arch=x86_64 --force-clean \
    --jobs=4 \
    --state-dir=/tmp/build-tmp/state-dir \
    /tmp/build-tmp/build-dir \
    org.ji.test.json
```

* **`flatpak-builder`**: The tool that automates downloading and compiling everything in the manifest.
* **`--state-dir`**: Directs the massive download cache and git metadata to the RAM disk.
* **`/tmp/build-tmp/build-dir`**: The folder where the "ready-to-use" libraries are installed.
* **`org.ji.test.json`**: The "recipe" file containing all dependencies.
* **`--jobs=4`**: Limits the number of simultaneous compilation tasks to 4. This prevents the system from running out of RAM during heavy C++ compilation phases.

Once Success `/tmp/build-tmp/build-dir` can be moved to SSD, It only contains the dependency compiled binary, By moving to SSD we save the binary and reuse from next time.
Also It is ok to place this in `tmp`
---

### Phase 3: The Developer Shell (The Inner Sandbox)
To compile the actual application code, one must enter the sandbox where the libraries exist.

```bash
flatpak-builder --run \
    --filesystem=/tmp/build-tmp \
    --filesystem=host \
    --share=network \
    /tmp/build-tmp/build-dir org.ji.test.json bash
```

* **`--run ... bash`**: Opens a terminal inside the container. Inside this terminal, the environment thinks it is a "clean" GNOME system.
* **`--filesystem=/tmp/build-tmp`**: Maps the RAM disk into the sandbox. (Consider a fresh directory, even if contains build-dir of dependencies not a problem)
* **`--filesystem=host`**: Allows the sandbox to see the source code stored on the physical SSD.
* **`--fshare=network`**: Grants the sandbox access to the internet. Needs for git clone or download anything
* **`/tmp/build-tmp/build-dir`**: Where the dependencies are installed, these content are copied to readonly section
---

### Phase 4: Manual Compilation (Inside the Shell)
Once inside the flatpak `--run` shell, the application is compiled manually using **Meson** and **Ninja**.

`cd /media/ssd/Project/open_source/gnome-monitor/gnome-system-monitor` change the path to application code directory 

1.  **Set Paths**: Tell the system to look for the newly built libraries in `/app`.
    ```bash
    export PKG_CONFIG_PATH="/app/lib64/pkgconfig:/app/lib/pkgconfig:/usr/lib64/pkgconfig:/usr/lib/pkgconfig"
    ```
2.  **Configure (Meson)**: Prepare the build.
    ```bash
    meson setup /tmp/build-tmp/build --buildtype debug -Ddevelopment=true --prefix=/tmp/build-tmp/install
    ```
3.  **Compile and Install (Ninja)**:
    ```bash
    ninja -C /tmp/build-tmp/build install
    ```

---

### Phase 5: Debugging and Execution
To run the newly compiled application with full debugging capabilities:

1.  **Set Schemas**: Tells the app where to find its settings/configurations.
    ```bash
    export GSETTINGS_SCHEMA_DIR=/tmp/build-tmp/install/share/glib-2.0/schemas
    ```
2.  **Start Debug Server**: Allows an external debugger (like VS Code) to attach.
    ```bash
    gdbserver :12345 /tmp/build-tmp/install/bin/gnome-system-monitor
    ```

---

### Summary of Component Roles

* **`tmpfs`**: The "Scratchpad" (RAM disk). Used for all temporary build files.
* **`flatpak-builder`**: The "Environment Manager." Builds the foundational libraries.
* **`build-dir`**: The "Local Library Store." Contains the stable dependencies.
* **`meson`**: The "Architect." Analyzes the source code and plans the build.
* **`ninja`**: The "Worker." Executes the actual compilation of C++/C files.
* **`gdbserver`**: The "Bridge." Connects the running app to the code editor for step-by-step debugging.

---

### Maintenance Notes
* **Rebooting**: If the computer reboots, Phase 1 must be repeated. The `build-dir` should be copied from SSD back to RAM to avoid re-compiling all dependencies.
* **Manifest Changes**: If `org.ji.test.json` is modified (adding a new library), Phase 2 must be re-run.
* **Code Changes**: If only the application `.cpp` or `.h` files are changed, only Phase 4 (Ninja step) needs to be repeated.


----
### Understanding Flatpak and Flatpak-Builder

To understand how this system works, imagine the difference between a **library** and a **toolbox**. 

On a standard Linux system (Host), libraries are shared. If one app needs a specific version of a tool and another app needs a different version, they often conflict. Flatpak solves this by giving every application its own "toolbox" (Sandbox).

---

### 1. What is Flatpak?
**Flatpak** is a deployment and sandboxing framework. It allows an application to run in an isolated environment, separate from the main Operating System (the Host).

* **The Runtime (Base Layer):** Every Flatpak uses a "Runtime" (like `org.gnome.Sdk`). This is a bundled set of basic libraries and tools that the app expects to find. 
* **The Sandbox:** The application runs inside a restricted container. By default, it cannot see your personal files, your webcam, or your network unless you explicitly give it permission.



---

### 2. What is Flatpak-Builder?
**Flatpak-Builder** is the developer's tool used to "cook" the application. It reads a manifest file (`.json`) and performs the following:
1.  **Downloads** all source code mentioned in the manifest.
2.  **Compiles** that code inside a temporary sandbox.
3.  **Installs** the result into a specific directory (the `build-dir`).
4.  **Bundles** the metadata so the system knows how to run the app later.

---

### 3. How does it work? (The Internal View)
When a Flatpak environment is created, it uses **Containerization Technology**. 

* Inside the sandbox, the filesystem is reorganized. The application thinks the libraries it needs are located in `/usr` or `/app`, even though those files are actually stored in a special folder on your SSD.
* It uses a "Union Filesystem." This overlays the **Runtime** (the base tools) with the **App** (your code) to create a single, unified view for the program.

---

### 4. Are the Binary (bin) files of the Host used?
**No.** This is the most important part of the isolation.

* **Independent Binaries:** The binaries on your host (like `/usr/bin/gcc` or `/usr/bin/python`) are **not** used inside the Flatpak sandbox. 
* **SDK Binaries:** When you build or run a Flatpak, the system uses the binaries provided by the **SDK** (Software Development Kit) you specified in the manifest. 
* **Reasoning:** This ensures that even if you upgrade your Host OS from Ubuntu 22.04 to 24.04, the Flatpak will still work perfectly because it carries its own compatible binary tools inside its sandbox.



---

### Summary Checklist

| Component | Description | Uses Host Binaries? |
| :--- | :--- | :--- |
| **Host OS** | Your main system (Ubuntu, Fedora, etc.). | Yes (for host tasks). |
| **Flatpak Runtime** | The base "OS" inside the container. | **No.** It uses its own. |
| **Flatpak-Builder** | The automated construction tool. | **No.** It uses the SDK's tools. |
| **The Sandbox** | The isolated "room" where the app runs. | **No.** It is invisible to the host. |

**Key takeaway for your workflow:** When you are inside the "Developer Shell," you are using the `meson`, `ninja`, and `g++` provided by the **GNOME Sdk**, not the ones installed on your computer. This is why your project stays consistent and never breaks your main system.

To understand the difference between these two commands, you have to distinguish between **The Construction Phase** (building the box) and **The Operation Phase** (stepping inside the box).

---

## 1. The Build Command (`flatpak-builder ...`)
This command is the **Factory**. It creates the environment.

* **Where it downloads:** Into `--state-dir` (`/tmp/build-tmp/state-dir`). This contains the raw source code (Git clones, `.tar.gz` files) and the build cache.
* **Where it installs:** Into the path provided before the json file (`/tmp/build-tmp/build-dir`). This becomes the **root** of your sandbox (the `/app` folder).
* **Permissions (Read/Write):** 
    * **Write Access:** It has full write access to both the `state-dir` and the `build-dir`.
    * **Read-Only:** The **SDK** (the base tools like `gcc` provided by GNOME) is mounted as read-only to ensure the base system isn't accidentally modified.

---

## 2. The Run Command (`flatpak-builder --run ...`)
This command is the **Developer Shell**. It opens the box you just built.

* **Where it downloads:** **Nowhere.** This command does not download anything. It expects the files to already exist in the `build-dir`.
* **Where it installs:** **Nowhere.** It simply opens a `bash` terminal inside the existing environment.
* **Permissions (Read/Write):**
    * **Read-Only:** The `/app` directory (which is your `build-dir` on the SSD/RAM) is usually mounted as **Read-Only** inside this shell. This is a security feature to ensure you don't manually mess up the dependencies you just spent time building.
    * **Write Access:** 
        * Because of `--filesystem=/tmp/build-tmp`, you have write access to your RAM disk.
        * Because of `--filesystem=host`, you have write access to your source code on the SSD.



---

## 3. Key Differences: Build vs. Run

| Feature | `flatpak-builder` (Build) | `flatpak-builder --run` (Run) |
| :--- | :--- | :--- |
| **Purpose** | To compile dependencies from the JSON manifest. | To work on your own code using those dependencies. |
| **Activity** | Heavy CPU and Disk usage (Downloading/Compiling). | Low usage (waiting for your input). |
| **Internal `/app`** | **Writable.** Files are being moved here. | **Read-Only.** It is a "frozen" environment. |
| **Primary Target** | The `build-dir`. | Your terminal/bash session. |

---

## 4. Why your workflow uses both
1.  You run the **Build** command to get things like `gtkmm` and `libadwaita` compiled and "installed" into your RAM disk (`build-dir`).
2.  Once that is finished, those libraries are "frozen" in that folder.
3.  You then use the **Run** command to enter a shell where those libraries are available to you. 
4.  Inside that shell, you manually run `meson` and `ninja` to build **your specific app** (`gnome-system-monitor`). You save your app's temporary build files in a separate folder (like `/tmp/build-tmp/build`) where you **do** have write access.



### Summary for your notes:
* **`state-dir`**: The "Kitchen" (Messy, downloads, source code).
* **`build-dir`**: The "Plate" (Clean, finished libraries, ready to serve).
* **`--run`**: The "Dining Room" (Where you use the libraries on the plate to build your own project).



# Flatpak Application Directory Structure

A Flatpak application follows a standardized directory layout to ensure compatibility across different Linux distributions. This structure is primarily divided into two areas: the **Internal Sandbox View** (what the application sees) and the **Storage View** (where the files actually exist on the disk).

---

## 1. The Internal Sandbox View (`/app`)

When an application runs inside a Flatpak sandbox, it is isolated from the Host OS system folders (like `/usr` or `/lib`). Instead, it is given its own dedicated root directory called `/app`.

| Directory | Content |
| :--- | :--- |
| **`/app/bin`** | Contains the primary executable files (binaries) of the application. |
| **`/app/lib`** or **`/app/lib64`** | Contains the compiled library files (`.so` files) that the application specifically needs. |
| **`/app/include`** | Contains C/C++ header files (`.h`). These are used during the build process but are often removed in final production versions. |
| **`/app/share`** | Contains non-binary data, such as icons, desktop files, translations (locales), and GSettings schemas. |
| **`/app/etc`** | Contains configuration files specific to the application. |

**The Logic of `/app`:**
The `/app` directory is the **prefix**. When software is compiled for Flatpak, it must be configured to look for its internal parts relative to `/app`. If a binary looks for a library in `/usr/lib` (the host path), it will fail because the sandbox hides the host's libraries.

---

## 2. The Runtime View (`/usr`)

While the application code lives in `/app`, the basic operating system tools (like `bash`, `ls`, or basic C libraries) live in `/usr`.

* **Source:** These files do not come from the Host OS. They are provided by the **Runtime** (e.g., `org.gnome.Platform`).
* **State:** This directory is **Read-Only**. An application cannot modify its runtime environment.

---

## 3. The Physical Storage View (On the SSD)

On a physical disk, Flatpak organizes files in a content-addressable system, similar to how Git stores data. This allows different applications to share the same library files on disk to save space.

### 3.1 System-Wide Installation
Files installed for all users are located at:
`/var/lib/flatpak/`

### 3.2 User-Specific Installation
Files installed only for the current user are located at:
`~/.local/share/flatpak/`

### 3.3 Internal Layout of Storage
Inside the storage folders above, the structure follows this pattern:

* **`app/[AppID]/current/active/files/`**: This directory contains the actual folders (`bin`, `lib`, `share`) that will be mapped to `/app` inside the sandbox.
* **`runtime/[RuntimeID]/current/active/files/`**: This contains the folders mapped to `/usr` inside the sandbox.
* **`export/`**: Contains files that the Host OS needs to see, such as the application icon and the `.desktop` file (so the app appears in the start menu).

---

## 4. User Data and Configuration

Because the `/app` and `/usr` directories are usually read-only or reset during updates, an application saves user-specific data (like game saves or browser history) in a writable location on the Host SSD:

`~/.var/app/[AppID]/`

* **`~/.var/app/[AppID]/config`**: Mapped to the application's configuration home.
* **`~/.var/app/[AppID]/cache`**: Mapped to temporary cache files.
* **`~/.var/app/[AppID]/data`**: Mapped to persistent application data.

---

## 5. Summary Table: Directory Mapping

| Inside Sandbox Path | Physical Host Path (Example) | Permissions |
| :--- | :--- | :--- |
| **`/app`** | `.../flatpak/app/[AppID]/active/files/` | Read-Only (Usually) |
| **`/usr`** | `.../flatpak/runtime/[RuntimeID]/active/files/` | Read-Only |
| **`/home`** | Restricted or `~/.var/app/[AppID]` | Writable |
| **`/tmp`** | A virtual temporary space in RAM | Writable |

**Conclusion:**
Flatpak uses a "layered" filesystem. It takes a **Runtime** (the bottom layer), adds the **Application** (the middle layer), and attaches **User Data** (the top layer). To the application, these all appear as one continuous system, but physically, they are stored separately to allow for easy updates and security isolation.
