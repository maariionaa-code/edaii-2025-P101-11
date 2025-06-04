# Report: Building a search engine like Google

> [!NOTE]  
> Complete the report in this file before delivering the project.
> The lab guide has a list of the questions you must answer.

1. C4 component diagram showing the data structures
![Report drawio (1)](https://github.com/user-attachments/assets/b8b5d6a8-e1ce-4598-b0f3-870ac0537868)

2. Table with 3 columns (description, Big-O, justification)
<img width="613" alt="Captura de pantalla 2025-06-02 a les 10 29 41" src="https://github.com/user-attachments/assets/878a3349-0d2f-4314-9ac8-5e4c79d7ff50" />

3. Search time: reverse index vs linear search
![Search Time](https://private-user-images.githubusercontent.com/170513247/451402456-2bc81d39-5b53-47bc-92de-57441a7b44b0.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3NDkwNDUzODEsIm5iZiI6MTc0OTA0NTA4MSwicGF0aCI6Ii8xNzA1MTMyNDcvNDUxNDAyNDU2LTJiYzgxZDM5LTViNTMtNDdiYy05MmRlLTU3NDQxYTdiNDRiMC5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjUwNjA0JTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI1MDYwNFQxMzUxMjFaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT05Nzc4Y2NkOGEzYzI0ZWM2YWJmNTI1ZmM5NjY1N2IwOTZhMTYyYWY2ZDg1ZTE2YTljMmU2NmRhNjlkOWY0MTExJlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCJ9.Xha6eBbuLsSEnRlNkBYrDz1_XTtz-JLAJqVF7R5qALI)


The results show that the search time with a reverse index remains almost constant, even as the number of documents increases significantly. In contrast, linear search time grows steadily, demonstrating the efficiency of using a reverse index in large datasets.

4. Initialization time for different hashmap slot count settings 

![Init time](https://private-user-images.githubusercontent.com/170513247/451446506-8903dd6e-730c-4579-8d9b-db831ac4f344.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3NDkwNTEwNTAsIm5iZiI6MTc0OTA1MDc1MCwicGF0aCI6Ii8xNzA1MTMyNDcvNDUxNDQ2NTA2LTg5MDNkZDZlLTczMGMtNDU3OS04ZDliLWRiODMxYWM0ZjM0NC5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjUwNjA0JTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI1MDYwNFQxNTI1NTBaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT00ODVhMWMyYjFkNzRiZWE1OTk4NGQ5ZmMxNDUwMWE1MmVhMDQ2NmY1NGI3NjZhNmY1NDA5Njk5NzIzMjI3YzQ1JlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCJ9.sxOw7I8jtRw0FRiNjeF7vz7vHAaHLPz92ykXJzO2Qow)

Increasing the number of hashmap slots significantly reduced the initialization time, especially when going from 100 to 1000 slots, due to fewer collisions and faster insertions. However, beyond 5000 slots, the performance gain was marginal, suggesting a trade-off between memory usage and speed.

6. Describe an improvement of the reverse index to improve search and initialization speed
To improve the reverse index's performance in both search and initalization, a well-justified solution is to replace the traditional hash map with a trie (prefix tree) structure. A trie organizes words by their characters ina tree-like hierarchy, enabling fast lookup, especially for words sharing common prefixes. This structure reduces redundancy in memory by storing shared prefixes only once, which significantly lowers the memory footprint in datasets with many similar or related terms. While the average-case lookup time in a hash map is O(1), the trie offers a predictable O(L) time, where L is the length of the keyword—still very efficient for practical purposes. Furthermore, a trie naturally supports prefix-based and fuzzy queries, which are not possible with a standard hash map, enabling advanced search features like autocomplete or spell-correction. Initialization time remains O(W × D) in both cases, where W is the number of unique words and D is the average number of documents per word. However, the trie structure may have slightly higher constant overhead during insertion. In summary, switching to a trie improves scalability and memory efficiency, especially in larger datasets with overlapping vocabulary, while offering more flexible and powerful query capabilities with only a minimal impact on lookup performance.
