# Report: Building a search engine like Google

1. C4 component diagram showing the data structures
![Report drawio (1)](https://github.com/user-attachments/assets/b8b5d6a8-e1ce-4598-b0f3-870ac0537868)

2. Table with 3 columns (description, Big-O, justification)
<img width="613" alt="Captura de pantalla 2025-06-02 a les 10 29 41" src="https://github.com/user-attachments/assets/878a3349-0d2f-4314-9ac8-5e4c79d7ff50" />

3. Search time: reverse index vs linear search

![search_time png](https://github.com/user-attachments/assets/a3739004-75f3-43a7-bc89-4f8eda7b8caf)

The results show that the search time with a reverse index remains almost constant, even as the number of documents increases significantly. In contrast, linear search time grows steadily, demonstrating the efficiency of using a reverse index in large datasets.

4. Initialization time for different hashmap slot count settings 

![initialization time](https://github.com/user-attachments/assets/8d8c45b7-b8b5-44ca-8254-d8ed4b55bbbd)

Increasing the number of hashmap slots significantly reduced the initialization time, especially when going from 100 to 1000 slots, due to fewer collisions and faster insertions. However, beyond 5000 slots, the performance gain was marginal, suggesting a trade-off between memory usage and speed.

5. Search time for different hashmap slot count settings

![search time](https://github.com/user-attachments/assets/f7864186-e985-4613-9a0d-a5d25bb7c8fa)

Search time decreased significantly as the hashmap slot count increased, especially between 100 and 1000 slots, due to a reduction in collisions during keyword lookup. However, beyond 5000 slots, improvements were minimal, indicating diminishing returns in search speed for higher capacities.

6. Describe an improvement of the reverse index to improve search and initialization speed
To improve the reverse index's performance in both search and initalization, a well-justified solution is to replace the traditional hash map with a trie (prefix tree) structure. A trie organizes words by their characters ina tree-like hierarchy, enabling fast lookup, especially for words sharing common prefixes. This structure reduces redundancy in memory by storing shared prefixes only once, which significantly lowers the memory footprint in datasets with many similar or related terms. While the average-case lookup time in a hash map is O(1), the trie offers a predictable O(L) time, where L is the length of the keyword—still very efficient for practical purposes. Furthermore, a trie naturally supports prefix-based and fuzzy queries, which are not possible with a standard hash map, enabling advanced search features like autocomplete or spell-correction. Initialization time remains O(W × D) in both cases, where W is the number of unique words and D is the average number of documents per word. However, the trie structure may have slightly higher constant overhead during insertion. In summary, switching to a trie improves scalability and memory efficiency, especially in larger datasets with overlapping vocabulary, while offering more flexible and powerful query capabilities with only a minimal impact on lookup performance.
