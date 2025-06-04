# Report: Building a search engine like Google

> [!NOTE]  
> Complete the report in this file before delivering the project.
> The lab guide has a list of the questions you must answer.

1. C4 component diagram showing the data structures
![Report drawio (1)](https://github.com/user-attachments/assets/b8b5d6a8-e1ce-4598-b0f3-870ac0537868)

2. Table with 3 columns (description, Big-O, justification)
<img width="613" alt="Captura de pantalla 2025-06-02 a les 10 29 41" src="https://github.com/user-attachments/assets/878a3349-0d2f-4314-9ac8-5e4c79d7ff50" />

3. Search time: reverse index vs linear search
![search_time](![Image](https://github.com/user-attachments/assets/3477aaf8-a598-42ff-9a85-8d5f20162d73))

The results show that the search time with a reverse index remains almost constant, even as the number of documents increases significantly. In contrast, linear search time grows steadily, demonstrating the efficiency of using a reverse index in large datasets.

6. Describe an improvement of the reverse index to improve search and initialization speed
To improve the reverse index's performance in both search and initalization, a well-justified solution is to replace the traditional hash map with a trie (prefix tree) structure. A trie organizes words by their characters ina tree-like hierarchy, enabling fast lookup, especially for words sharing common prefixes. This structure reduces redundancy in memory by storing shared prefixes only once, which significantly lowers the memory footprint in datasets with many similar or related terms. While the average-case lookup time in a hash map is O(1), the trie offers a predictable O(L) time, where L is the length of the keyword—still very efficient for practical purposes. Furthermore, a trie naturally supports prefix-based and fuzzy queries, which are not possible with a standard hash map, enabling advanced search features like autocomplete or spell-correction. Initialization time remains O(W × D) in both cases, where W is the number of unique words and D is the average number of documents per word. However, the trie structure may have slightly higher constant overhead during insertion. In summary, switching to a trie improves scalability and memory efficiency, especially in larger datasets with overlapping vocabulary, while offering more flexible and powerful query capabilities with only a minimal impact on lookup performance.
