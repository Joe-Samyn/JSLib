# Design & Engineering Notes

## Buddy Allocator
A buddy allocator is a memory management technique that focuses on optimizing allocations and reducing fragmentation. It is a suprisingly simple, yet very power algorithm for allocating and managing memory. 

### The Algorithm
As mentioned, the buddy allocation technique focuses on powers of 2. The programmer will determine the smallest order of memory that can be allocated and the largest order of memory that can be allocated. The smalles order is called min_order and the largest order is max_order. The min_order is proportional to 2^(min_order) and the max_order is proportional to 2^(max_order). When allocations occur, the blocks are continually split in half until a block that is closest to the desired size is obtained. When blocks are split, the retain a reference to the other half or 'buddy'. This buddy system is used to coalesce memory during deallocation operations and reduce fragmentation. During a deallocation, if a block is freed whose buddy is also free, they can be rejoined into a single block. 

### Buddy Allocator Interface

#### Initialization

#### Allocation

#### Deallocation
