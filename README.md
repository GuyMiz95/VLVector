# VLVector

# Introduction
The VLVector class template provides a flexible, dynamic array-like data structure with optimized storage. It automatically switches between stack-allocated storage (static) and heap-allocated storage (dynamic) based on the number of elements and a predefined static capacity (C). This design allows for efficient use of memory and can help reduce heap allocations when the number of elements is small.

# Features
Automatic Storage Management: Uses stack-based storage by default and switches to heap-based storage as needed.
Customizable Capacity: The initial static capacity can be defined through the template parameter.
Iterator Support: Includes support for standard iterators, including reverse iterators.
Exception Safe: Provides strong exception safety guarantees with no memory leaks.
Element Access: Offers direct element access functions, such as At() which throws std::out_of_range for invalid indices.
Modifiers: Includes methods for inserting, erasing, and clearing elements, adjusting capacity dynamically as needed.
