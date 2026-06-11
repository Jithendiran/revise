#include <iostream>

// Defining isolated engine namespaces
namespace Audio {
    struct System { int volume; };
    
    void configure(System& sys) {
        std::cout << "[Audio System] Configuring volume to: " << sys.volume << "\n";
    }
}

namespace Graphics {
    struct System { int resolution_x; };
    
    void configure(System& sys) {
        std::cout << "[Graphics System] Configuring resolution to: " << sys.resolution_x << "p\n";
    }
}

// Global scope fallback function
void configure() {
    std::cout << "[Global System] Running global system fallback diagnostic.\n";
}

int main() {
    std::cout << "=== PHASE 1.3: NAMESPACES & ADL DEMONSTRATION ===\n\n";

    // 1. Explicitly resolving names using the scope resolution operator (::)
    Audio::System my_audio{100};
    Graphics::System my_video{1080};

    Audio::configure(my_audio);
    Graphics::configure(my_video);

    // 2. Calling the global scope explicitly
    ::configure(); 

    std::cout << "\n------------------------------------------------\n";
    std::cout << "Demonstrating Argument-Dependent Lookup (ADL):\n";
    
    // 3. Notice we do NOT type "Audio::configure" here.
    // Because 'my_audio' belongs to the 'Audio' namespace, 
    // ADL automatically finds the correct function!
    configure(my_audio); 

    return 0;
}

// g++ -std=c++20 main.cpp -o /tmp/namespace_test