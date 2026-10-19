*This project has been created as part of the 42 curriculum by dmupindu.*

# Codexion

## Description

Codexion is a concurrent programming project developed as part of the 42 curriculum. It explores multithreading, synchronization, resource sharing, and scheduling using the POSIX threads (pthreads) library in C.

The simulation consists of coders arranged in a circular configuration, sharing a set of dongles. Each coder must acquire both adjacent dongles before compiling. After compiling, the coder releases the dongles, then proceeds through debugging and refactoring before requesting dongles again.

The program supports two scheduling policies:

- **FIFO (First In, First Out):** prioritizes requests according to their arrival order.
- **EDF (Earliest Deadline First):** prioritizes eligible requests according to their deadlines.

The project focuses on coordinating concurrent threads while preventing deadlocks, managing dongle cooldown periods, detecting coder burnout, and ensuring that shared state and log output are accessed safely.

## Instructions

### Requirements

- A C compiler supporting POSIX threads.
- `make`.
- A POSIX-compatible environment, such as Linux.

### Compilation

From the root of the repository, compile the project using:

```bash
make
```

To clean and rebuild the project, use:

```bash
make re
```

### Execution

Run the program using the following argument format:

```bash
./codexion number_of_coders time_to_burnout \
time_to_compile time_to_debug time_to_refactor \
number_of_compiles dongle_cooldown scheduler
```

Arguments:

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders in the simulation; at least 2 |
| `time_to_burnout` | Maximum time a coder can go without compiling before burnout |
| `time_to_compile` | Duration of a compilation |
| `time_to_debug` | Duration of debugging |
| `time_to_refactor` | Duration of refactoring |
| `number_of_compiles` | Required number of compilations per coder |
| `dongle_cooldown` | Time a dongle remains unavailable after release |
| `scheduler` | Scheduling policy: `fifo` or `edf` |

All time values are expressed in milliseconds. Numeric arguments must satisfy the program's validation rules.

Example using FIFO:

```bash
./codexion 4 1000 200 200 200 5 50 fifo
```

Example using EDF:

```bash
./codexion 4 1000 200 200 200 5 50 edf
```

The simulation ends when the termination conditions are met, such as a coder burning out or all coders completing the required number of compilations.

## Blocking cases handled

### 1. Deadlock prevention

Each coder requires two dongles to compile. If threads acquire dongle mutexes in different orders, circular lock dependencies could arise.

To prevent this, the implementation locks the two dongle mutexes in ascending dongle-ID order. This consistent lock ordering prevents circular wait among dongle mutex acquisitions.

Both dongles are checked and reserved together while their mutexes are held. A coder does not reserve one dongle and wait while holding it for the other.

### 2. Head-of-line blocking

The scheduler must not become stuck simply because the highest-priority request currently cannot acquire its dongles.

Instead of considering only the heap's top request, the scheduler searches for the highest-priority request whose required dongles are eligible. This allows other coders to proceed when their resources are available.

The scheduling policy still determines priority among eligible requests.

### 3. Starvation considerations

The scheduler skips requests whose dongles are unavailable, allowing eligible requests to proceed. This prevents a blocked request at the head of the queue from unnecessarily stopping the entire simulation.

However, skipping blocked requests alone does not guarantee that every request will eventually be selected under every workload. Strict starvation freedom depends on the scheduling policy and workload.

### 4. Dongle cooldown

After a coder releases its dongles, each dongle receives an `available_at` timestamp based on the release time and configured cooldown.

The scheduler checks this timestamp before reserving the dongle. A dongle is eligible only when it is not in use and its cooldown has expired.

### 5. Burnout detection

The monitor checks coder deadlines and detects when a coder has exceeded the permitted time without compiling.

The simulation uses millisecond timestamps. Log timestamps are expressed relative to the simulation start, while internal deadline calculations use consistent absolute millisecond values.

The monitor and other threads coordinate access to shared termination state using synchronization.

### 6. Log serialization

Multiple threads may attempt to print messages at the same time. A dedicated print mutex protects log output so that messages from different threads do not interleave.

### 7. Race-condition prevention

Shared state, including dongle availability, scheduler requests, termination state, and compilation counts, must be accessed using the appropriate mutex protection. Condition-variable waits use their associated mutex to coordinate waiting and state changes safely.

The implementation has been tested using Helgrind to help identify data races, lock-ordering problems, and misuse of POSIX threading primitives.

## Thread synchronization mechanisms

### `pthread_mutex_t`

Mutexes protect shared data from concurrent modification.

- **Dongle mutexes:** each dongle has its own mutex protecting its `in_use` and `available_at` state.
- **Scheduler mutex:** protects scheduler state, including the request queue, request-granting coordination, and shared termination-related state.
- **Print mutex:** serializes log output from different threads.

For example, when the scheduler reserves two dongles, it locks both dongle mutexes in ascending ID order, checks their availability, marks both as in use if eligible, and then unlocks them. This prevents concurrent reservations from claiming the same dongle.

### `pthread_cond_t`

Condition variables allow threads to wait for state changes instead of continuously waiting for a notification.

The scheduler condition variable coordinates the scheduler and requesting coder threads. When a request is submitted, the scheduler is notified. When the scheduler grants a request or the simulation stops, waiting threads are notified so they can recheck the relevant state.

Condition-variable waits are performed with the associated mutex, and the waiting thread checks its condition in a loop after waking.

### Monitor and termination coordination

The monitor checks coder deadlines and updates the shared stop state when burnout is detected. Access to the stop state is synchronized with the scheduler mutex, and condition-variable broadcasts notify threads that need to respond to termination.

The main thread joins the worker threads before destroying shared synchronization objects and freeing shared data.

### Custom event implementation

This implementation uses POSIX mutexes and condition variables for thread coordination. It does not rely on a separate custom event primitive unless one is explicitly implemented elsewhere in the source code.

## Resources

### POSIX threads and synchronization

- [POSIX Threads in Operating Systems — GeeksforGeeks](https://www.geeksforgeeks.org/operating-systems/posix-threads-in-os/)
- [POSIX Threads (Pthreads): A Simple Explanation of Multithreading in C](https://medium.com/@techdhaba.training/posix-threads-pthreads-the-simplest-way-to-understand-real-multithreading-in-c-c2f591ab7a03)
- [The Open Group — POSIX Threads](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
- [Beej's Guide to C Programming](https://beej.us/guide/bgc/) — useful supplementary C reference.

### Videos

- [YouTube: POSIX threads introduction](https://www.youtube.com/watch?v=ldJ8WGZVXZk&t=176s)
- [YouTube: Pthreads tutorial series](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)

### Concurrency, deadlocks, and operating systems

- [Operating Systems: Three Easy Pieces (OSTEP)](https://pages.cs.wisc.edu/~remzi/OSTEP/) — free textbook with chapters on concurrency, locks, condition variables, and thread coordination.
- [Coffman Conditions and Deadlock Prevention](https://www.techlearnbase.com/tutorials/operating-systems/06-deadlocks/) — an introduction to the four necessary conditions for deadlock.

### Testing and debugging

- [Valgrind Helgrind Manual](https://valgrind.org/docs/manual/hg-manual.html) — documentation for detecting threading errors, data races, and potential deadlocks.
- [Valgrind User Manual](https://valgrind.org/docs/manual/manual.html) — general reference for memory and concurrency debugging tools.

### Use of artificial intelligence

AI tools were used as supplementary learning and development aids during this project. They helped explain POSIX threading concepts, review synchronization and resource-reservation logic, investigate head-of-line blocking, reason about race conditions and deadlock prevention, interpret Valgrind and Helgrind results, and improve documentation.

AI assistance was also used to suggest test scenarios and review edge cases involving scheduler selection, dongle cooldown, burnout detection, thread shutdown, and memory cleanup. The implementation was tested and refined through compilation, execution, and debugging.

AI-generated explanations and suggestions were treated as material to evaluate and verify rather than as a substitute for understanding the code. The final disclosure should reflect the actual assistance used throughout the project.