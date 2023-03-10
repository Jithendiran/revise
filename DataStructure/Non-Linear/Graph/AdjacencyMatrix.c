/**
 * @author: E.K.Jithendiran
 * Date: 10.03.2023
 * Graph
 * Time complexity: O(n^2)
 * Space Complexity: O(n^2)
 */
#include <stdio.h>

/**
 * This fuction will check 1D array has element
 * @param arr 1D array reference
 * @param size size of the array
 * @param element element to be find
 * @returns index of that element else -1
 */
int hasElement(int *arr, int size, int element)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == element)
            return i;
    }

    return -1;
}

/**
 * This function will traverse the graph by BFS
 * @param e no.of element
 * @param a pointer to array
 * @param start start element
 */
void bfs(int e, int a[][e], int start)
{
    // BFS -> BREATH FIRST SEARCH
    // BFS uses queue
    int queue[e];
    for (int i = 0; i < e; i++)
    {
        queue[i] = -1;
    }

    int top = 0;
    int slider = 0;
    // Method 1 start from 0th index to n
    // for (int i = 0; i < e; i++)
    // {
    //     if (hasElement(queue, e, i) == -1)
    //     {
    //         queue[top] = i;
    //         top++;
    //     }
    //     for (int j = 0; j < e; j++)
    //     {
    //         if (a[i][j] == 1 && hasElement(queue, e, j) == -1)
    //         {
    //             queue[top] = j;
    //             top++;
    //         }
    //     }
    // }

    // Method 2 follows queue
    queue[top] = start;
    top++;

    for (slider; slider < top; slider++)
    {
        for (int j = 0; j < e; j++)
        {
            if (a[queue[slider]][j] == 1 && hasElement(queue, e, j) == -1)
            {
                queue[top] = j;
                top++;
            }
        }
    }

    printf("BFS Order of the Graph:\n");
    for (int i = 0; i < top; i++)
    {
        printf("%d\t", queue[i]);
    }
    printf("\n");
}

/**
 * This function will traverse the graph by DFS
 * @param a pointer to array
 * @param e no.of element
 * @param start start element
 */
void dfs(int *a, int e, int start)
{
    // DFS -> DEPTH FIRST SEARCH
    // DFS uses stack
    int top = -1;
    int index = 0;
    int stack[e];
    int visited[e];
    int printQueue[e];
    int printIndex = 0;
    for (int i = 0; i < e; i++)
    {
        stack[i] = -1;
        visited[i] = -1;
    }
    stack[++top] = start;
    visited[index++] = start;
    printQueue[printIndex++] = start;

    int i = top;
    while (i >= 0)
    {
        int flag = 0;
        for (int j = 0; j < e; j++)
        {
            int element = *((a + stack[i] * e) + j);
            if (element == 1 && hasElement(visited, e, j) == -1)
            {
                flag = 1;
                stack[++top] = j;
                visited[index++] = j;
                printQueue[printIndex++] = j;
                i = top;
                break;
            }
        }
        if (flag == 0)
        {
            --top;
            --i;
        }
    }

    printf("DFS Order of the Graph:\n");
    for (int i = 0; i < printIndex; i++)
    {
        printf("%d\t", printQueue[i]);
    }
    printf("\n");
}

void main()
{
    int e = 6;
    // every node or vertex has path to edge to other node
    int a[][6] = {
        // 0 1  2  3  4  5
        {0, 0, 1, 1, 0, 0}, // 0
        {0, 0, 0, 1, 1, 1}, // 1
        {1, 0, 0, 0, 0, 0}, // 2
        {1, 1, 0, 0, 0, 0}, // 3
        {0, 1, 0, 0, 0, 0}, // 4
        {0, 1, 0, 0, 0, 0}  // 5
    };
    // int a[][6] = {
    //     // 0 1  2  3  4  5
    //     {0, 0, 1, 1, 0, 0}, // 0
    //     {0, 0, 0, 1, 0, 0}, // 1
    //     {1, 0, 0, 0, 0, 0}, // 2
    //     {1, 1, 0, 0, 0, 0}, // 3
    //     {0, 0, 0, 0, 0, 1}, // 4
    //     {0, 1, 0, 0, 1, 0}  // 5
    // };
    // graph traversal
    bfs(e, a, 3);
    dfs((int *)a, e, 3);
}

/**
 * Spanning Tree
 * See: https://www.javatpoint.com/spanning-tree,https://www.youtube.com/watch?v=4ZlRH0eK-qQ
 */