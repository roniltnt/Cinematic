# Smart Cinema Management System
### Final Project — Object-Oriented Programming in C++

---

> **Course:** Object-Oriented Programming  
> **Language:** C++ (C++17, MSVC)  
> **Constraints:** No STL, no templates, manual memory management, Rule of Three, strict `const` correctness

---

## Table of Contents

1. [Part 1 — Project Selection & Code Critique](#part-1--project-selection--code-critique)
   - [1.1 Why We Chose Smart Cinema Management](#11-why-we-chose-smart-cinema-management)
   - [1.2 Rejected Project: Military Base Management System](#12-rejected-project-military-base-management-system)
   - [1.3 Rejected Project: Supply and Transaction Management System](#13-rejected-project-supply-and-transaction-management-system)
2. [Part 2 — Deviations, Fixes, and Justifications](#part-2--deviations-fixes-and-justifications)
   - [Deviation 1 — Separating Movie/Hall and Introducing `Screening`](#deviation-1--separating-moviehall-and-introducing-screening)
   - [Deviation 2 — Fully Automated, Contextual Ticket Purchasing](#deviation-2--fully-automated-contextual-ticket-purchasing)
   - [Deviation 3 — Dynamic 3D Capacity and Per-Screening Seat Tracking](#deviation-3--dynamic-3d-capacity-and-per-screening-seat-tracking)
   - [Deviation 4 — Auto-Initializing `visitCount` to Zero](#deviation-4--auto-initializing-visitcount-to-zero)
   - [Deviation 5 — Removing Club Points and `operator>`](#deviation-5--removing-club-points-and-operator)
   - [Deviation 6 — Strict Memory Management, `const` Correctness, and Rule of Three](#deviation-6--strict-memory-management-const-correctness-and-rule-of-three)
3. [Deviation Summary Table](#deviation-summary-table)

---

## Part 1 — Project Selection & Code Critique

### 1.1 Why We Chose Smart Cinema Management

After reviewing all available projects, we selected the **Smart Cinema Management** system because it presented the most balanced and intellectually honest use of C++ object-oriented principles. The system's domain maps naturally onto a class hierarchy without forcing artificial abstractions.

Specifically, the project demonstrated three qualities we considered essential for a credible OOP submission:

- **Meaningful polymorphism at multiple levels.** The `Guest` hierarchy (`Customer`, `Reviewer`) and the `Hall` hierarchy (`Hall3D`, `VIPHall`, `Hall3DVIP`) each form independent inheritance trees, giving the project two distinct sites where heterogeneous arrays, virtual dispatch, and polymorphic cloning are genuinely justified — not merely cosmetic.

- **A natural use case for the Rule of Three.** Because `Cinema` owns dynamic arrays of pointers to polymorphic objects, and because `Guest` owns a dynamic array of `Ticket*`, deep-copy semantics are a real requirement rather than a contrived exercise. Copy constructors, assignment operators, and destructors each carry substantive weight.

- **Clean separation of identity and behaviour.** Entities like `Movie`, `Hall`, and `Employee` have narrow, coherent responsibilities. No single class was forced to manage unrelated domains, which kept the inheritance graph legible and the destructor ordering straightforward.

The project also presented a well-scoped challenge: complex enough to require careful architectural decisions, yet self-contained enough that every class had a clear purpose within the system.

---

### 1.2 Rejected Project: Military Base Management System

**System Overview:** The Military Base Management System manages soldiers, officers, missions, and a vehicle fleet comprising `Jeep`, `Truck`, and `ArmoredTransport` classes.

We rejected this project after identifying fundamental C++ architectural problems that the design document either did not address or actively made worse.

#### Critical C++ Issue 1 — The Diamond Problem in the Vehicle Hierarchy

The central flaw is that `ArmoredTransport` inherits from *both* `Jeep` and `Truck`, both of which inherit from `Vehicle`. In standard C++, this creates the **diamond problem**: the compiler instantiates *two* independent `Vehicle` subobjects inside every `ArmoredTransport` instance — one via `Jeep` and one via `Truck`. This produces three concrete consequences:

1. **Ambiguous method resolution.** Any call to a method defined on `Vehicle` — such as `getFuelCapacity()` or `getRegistrationId()` — becomes a compile-time error unless explicitly qualified with the intermediate class name (e.g., `Jeep::getFuelCapacity()`). The original project's code does not qualify these calls, meaning it fails to compile correctly as written.

2. **Duplicated base class subobjects.** Without `virtual` inheritance, an `ArmoredTransport` object carries *two separate copies* of every `Vehicle` data member. This is not merely a memory concern; it creates a logically contradictory object where the `ArmoredTransport` has two different registration IDs and two different fuel tanks, neither of which reflects reality.

3. **Broken `operator=` and copy constructors.** The Rule of Three becomes nearly impossible to implement correctly across this diamond. A correct deep copy of `ArmoredTransport` must navigate two separate `Vehicle` subobjects, requiring explicit `virtual` inheritance, a custom virtual destructor chain, and careful initializer-list construction — none of which the original design accounted for.

The fix — `virtual public` inheritance at the `Jeep` and `Truck` level — requires restructuring the entire vehicle hierarchy and adding explicit virtual base constructors. The amount of remedial work required was not commensurate with the pedagogical value gained.

#### Critical C++ Issue 2 — Nested Dynamic Arrays and Rule of Three Complexity

The system's organisational structure requires `Officer` objects to own arrays of `Soldier*`, while `Unit` objects own separate arrays of `Soldier*` pointing to the same soldiers. This creates a **shared ownership problem** that, without `std::shared_ptr` (which is forbidden under course constraints), forces the student to manually implement reference-counted ownership or accept dangling pointers.

Specifically:

- If `Unit` is copied, its copy constructor must decide whether to deep-copy the `Soldier*` array (creating duplicated soldiers) or alias it (creating dangling pointers when either the original or the copy is destroyed).
- If an `Officer` is removed from a `Unit`, any `Soldier*` it owned that the `Unit` also holds becomes invalid without an explicit notification mechanism.
- The destructor ordering between `Unit` and `Officer` is non-trivial and cannot be resolved without a globally agreed-upon ownership contract that the original design does not specify.

This creates a **class of memory errors** — double-frees, use-after-free, and stale pointers — that emerge only at runtime under specific interaction sequences, making them extremely difficult to diagnose. We concluded that the design actively incentivises incorrect C++ rather than demonstrating mastery of it.

---

### 1.3 Rejected Project: Supply and Transaction Management System

**System Overview:** The Supply and Transaction Management System handles consumers, suppliers, products, and delivery operations, using `DeliveryVehicle` and `DeliveryGuy` as the two delivery mechanisms.

This project was rejected on two grounds: a fundamental violation of the **Single Responsibility Principle** baked into the class hierarchy, and a pattern of raw double-pointer usage that creates intractable exception-safety problems.

#### Critical C++ Issue 1 — Semantically Incoherent Multiple Inheritance

The design requires `DeliveryGuy` to inherit from *both* `Person` and `DeliveryMethod`. This is not a diamond-problem scenario, but it is a more insidious architectural error: **two base classes with incompatible conceptual contracts are merged into one concrete class**.

`Person` is an *identity type*: it has a name, an ID, and a lifespan tied to an individual human being. Its copy semantics model the idea of duplicating a record. `DeliveryMethod` is a *behavioural interface*: it describes how a delivery is executed, with no inherent identity. Its contract is closer to a strategy pattern than to an entity.

When these are merged into `DeliveryGuy`, several C++ problems emerge immediately:

1. **Copy semantics become undefined by design.** Copying a `DeliveryGuy` means copying both a person's identity *and* a delivery strategy. It is unclear whether the copy represents the same physical person (aliasing), a different person with the same delivery behaviour (deep copy of identity, shared behaviour), or something else entirely. The original code did not define a copy constructor for `DeliveryGuy`, defaulting to a shallow copy that aliases all pointer members — a memory safety failure.

2. **Destructor responsibility is ambiguous.** If `DeliveryMethod` owns any dynamically allocated resources (e.g., a route plan or a vehicle reference), and `Person` also owns dynamic memory (e.g., a `char*` name), the combined destructor must manage two independent resource hierarchies. Without explicit virtual destructors on *both* base classes, destroying a `DeliveryGuy` through a `DeliveryMethod*` pointer silently skips the `Person` destructor, leaking the name buffer.

3. **The hierarchy cannot be extended coherently.** Adding a `DeliveryDrone` (a non-person delivery method) requires it to share a base class with a human being, which is conceptually absurd and forces every polymorphic method that accepts `DeliveryMethod*` to be aware of whether the underlying object is also a `Person`.

#### Critical C++ Issue 2 — Unencapsulated Double-Pointer Matrices in `TransactionManager`

The original group's `TransactionManager` class stores pending transactions as a raw `Consumer**` matrix and pending stock as a `Product**` matrix, with no encapsulation layer around these double pointers.

Under the course constraint of no STL (meaning no `std::vector` or RAII wrappers), this pattern creates a severe **exception-safety gap**:

- During a multi-step transaction, if `new Product*[n]` succeeds but a subsequent `new Consumer*[m]` throws `std::bad_alloc`, the already-allocated product matrix is never freed. There is no destructor called on a partially constructed object because the `TransactionManager` was already fully constructed — the matrix allocation occurs inside a transaction method, not the constructor.
- The original code contained no `try`/`catch` around any of these allocation sequences, meaning a single allocation failure could leave the `TransactionManager` in a state where it holds a partially initialised matrix of dangling pointers, and subsequent calls proceed to dereference them.
- Iterating over a `Consumer**` matrix requires knowing both dimensions at all times; the original code passed these as unvalidated integers, creating the possibility of out-of-bounds reads when a transaction was cancelled mid-insertion.

Correcting these issues would require rewriting `TransactionManager` as a proper dynamic array manager with its own resize-and-copy logic — essentially reimplementing a safe 2D array class from scratch. We judged this scope to be beyond the project's core educational goal of demonstrating OOP, not data structure implementation.

---

## Part 2 — Deviations, Fixes, and Justifications

The following section documents every substantive modification we made to the original Cinema project's design. For each deviation, we explain the original authors' likely reasoning, classify the change as **Required** or **Recommended**, and provide a detailed technical justification.

---

### Deviation 1 — Separating Movie/Hall and Introducing `Screening`

| | |
|---|---|
| **Status** | ✦ Required |
| **Files affected** | `Movie.h`, `Hall.h`, `Screening.h` *(new)*, `Cinema.h`, `Cinema.cpp` |

**What the original authors designed:**  
The original `Movie` class stored a `Date premiereDate` as a direct member, implying that a movie has a single fixed date. The `Hall` class stored a `Movie* currentMovie` pointer, implying that a hall plays exactly one movie at any given time. The hall's `bool seats[100]` array tracked which seats were occupied for that one movie.

**Why this is architecturally incorrect:**  
This design conflates two distinct concepts — a *film title* and a *scheduled event* — into a single object. In reality, the same film is screened multiple times across multiple halls on multiple dates. The original design makes it structurally impossible to represent this without either duplicating `Movie` objects (creating inconsistent state) or destroying and recreating them (losing history).

Furthermore, placing `currentMovie` inside `Hall` and `seats[]` inside `Hall` means the hall object holds booking state that logically belongs to a specific scheduled event. If the same hall runs a morning and an evening show, the original design has nowhere to store both seat maps simultaneously.

**What we changed:**  
We removed `Date` from `Movie` entirely, leaving it as a pure descriptor (`title`, `lengthMinutes`, `is3D`). We removed `currentMovie*` and `seats[]` from `Hall`, leaving it as a pure venue descriptor. We introduced a new `Screening` class that combines:

- A `const Movie&` reference (non-owning)
- A `const Hall&` reference (non-owning)
- A `Date screeningDate`
- `startHour` and `startMinute`
- `totalSeats` and `soldTickets` for live seat tracking
- An `overlapsWith(const Screening&)` method for scheduling conflict detection

`Cinema` owns a `Screening**` array and is responsible for creating and validating all screenings through `scheduleScreening()`, which checks for time overlaps in the same hall before adding a new `Screening` to the array.

**Technical consequence:**  
This enforces correct relational data modeling. A `Movie` is a catalogue entry. A `Hall` is a physical venue. A `Screening` is the event that binds them at a point in time. The destructor ordering in `Cinema` must respect this: `Ticket*` objects (which hold `const Screening&`) must be destroyed before `Screening*` objects, and `Screening*` objects must be destroyed before `Movie*` and `Hall*` objects, since they hold non-owning references to those entities.

---

### Deviation 2 — Fully Automated, Contextual Ticket Purchasing

| | |
|---|---|
| **Status** | ★ Recommended |
| **Files affected** | `Cinema.h`, `Cinema.cpp`, `main.cpp` |

**What the original authors designed:**  
The original menu prompted the user to explicitly choose between a Regular ticket and a VIP ticket, and to confirm whether they wanted a 3D ticket. This was likely intended to demonstrate user interaction and polymorphic object creation via branching logic in `main`.

**Why this breaks encapsulation:**  
Allowing the user to freely choose a VIP ticket for any screening in any hall means the program can create a semantically invalid state: a `VIPTicket` issued for a screening held in a `Hall` that has no VIP services. The ticket object asserts something about the hall that the hall does not support. This is precisely the class of logical inconsistency that encapsulation is designed to prevent.

**What we changed:**  
We moved all ticket-type determination into `Cinema::sellTicket(int screeningIdx, int guestIdx)`. The method derives both flags from the `Screening`'s associated `Hall`:

```cpp
bool is3D  = sc->getIs3DScreening();
bool isVIP = (sc->getHall().getWaitersCount() > 0);
```

A hall that is not a `VIPHall` or `Hall3DVIP` returns `0` from the virtual `getWaitersCount()` method, so `isVIP` is `false` by construction. The user selects only the screening and the guest; the system determines what ticket type is appropriate. This is a **factory-like creation pattern**: the context (the `Screening` and its `Hall`) fully determines the concrete type of object produced, without the caller needing to specify it.

**Technical consequence:**  
The `sellTicket` signature was simplified from five parameters to two, pushing all ticket-type logic behind the `Cinema` interface. This means `main.cpp` contains zero branching on ticket type — all type-selection logic lives where it belongs, in the domain class.

---

### Deviation 3 — Dynamic 3D Capacity and Per-Screening Seat Tracking

| | |
|---|---|
| **Status** | ★ Recommended |
| **Files affected** | `Hall.h`, `Hall3D.h`, `Screening.h`, `Screening.cpp`, `Cinema.cpp` |

**What the original authors designed:**  
The original `Hall` class contained a `bool seats[100]` static array, representing a fixed 100-seat capacity where each element tracked individual seat occupancy. This was a simple and uniform model — all halls have 100 seats, all seats are equivalent.

**Why this is insufficient for 3D screenings:**  
A `Hall3D` has a finite number of 3D glasses (`glassesCount`). If only 50 pairs of glasses are available, scheduling a 3D screening in that hall must cap capacity at 50, not 100. The original model had no mechanism for this: a `Hall3D` with 50 glasses would appear to have 100 available seats, allowing 100 tickets to be sold while only 50 guests could actually receive glasses.

Additionally, a static `bool seats[100]` array stored in the `Hall` cannot represent multiple concurrent screenings. As established in Deviation 1, a hall may host multiple screenings. Each screening requires its own independent seat counter.

**What we changed:**  
We added a virtual `get3DGlassesCount()` method to the `Hall` base class (returning `0` by default), overridden in `Hall3D` to return `glassesCount`, and inherited correctly through `Hall3DVIP` via dominance. In `Cinema::scheduleScreening()`, we compute the effective seat count before constructing the `Screening`:

```cpp
int totalSeats = Hall::NUM_SEATS;  // 100
if (is3DScreening) {
    int glasses = hall->get3DGlassesCount();
    if (glasses > 0 && glasses < totalSeats) totalSeats = glasses;
}
```

The `Screening` object receives this pre-computed `totalSeats` value and tracks `soldTickets` independently. `sellSeat()` throws `std::runtime_error` if `soldTickets >= totalSeats`, making overbooking a caught exception rather than a silent logic error or array out-of-bounds access.

---

### Deviation 4 — Auto-Initializing `visitCount` to Zero

| | |
|---|---|
| **Status** | ✦ Required |
| **Files affected** | `Guest.h`, `Guest.cpp`, `Customer.h`, `Customer.cpp`, `Reviewer.h`, `Reviewer.cpp`, `main.cpp` |

**What the original authors designed:**  
The original `Guest` constructor accepted `int visitCount` as an explicit parameter, defaulting to `0`. The rationale was likely to demonstrate parameterised construction and provide flexibility for initialising a `Guest` object with a pre-existing visit history.

**Why this is a logical flaw:**  
In the context of a running cinema management system, a `Guest` is created at the moment they register. Their visit count at that moment is necessarily zero — they have not yet visited. Prompting the operator to enter a visit count during registration introduces a class of input errors that the system cannot distinguish from valid data. An operator who enters `5` creates a guest object that claims to have visited five times before any ticket has ever been sold to them, corrupting the semantics of `visitCount` entirely.

More broadly, this violates the C++ design principle that constructors should leave objects in a **valid and self-consistent state** without relying on the caller to supply trivially derivable values. A visit count of zero at registration is not an assumption — it is a logical certainty.

**What we changed:**  
We removed `visitCount` from all three constructors (`Guest`, `Customer`, `Reviewer`) and initialised it unconditionally to `0` in the `Guest` constructor's initialiser list:

```cpp
Guest::Guest(const char* name, int id)
    : Person(name, id), visitCount(0), ...
```

The `incrementVisitCount()` call inside `Guest::addTicket()` remains unchanged, ensuring the counter still advances correctly each time a ticket is purchased. The `visitCount` field, the getter, and the setter are all preserved — only the constructor parameter is removed.

---

### Deviation 5 — Removing Club Points and `operator>`

| | |
|---|---|
| **Status** | ★ Recommended |
| **Files affected** | `Customer.h`, `Customer.cpp`, `Ticket.h`, `Ticket.cpp`, `VIPTicket.h`, `VIPTicket.cpp` |

**What the original authors designed:**  
The original `Customer` class carried an `int clubPoints` member with associated `getClubPoints()`, `setClubPoints()`, and `addClubPoints()` methods. The original `Ticket` class implemented an `operator>` for price comparison and a `calcFinalPrice()` method with constants for base prices, 3D surcharges, and VIP surcharges.

The original authors likely added these features to fulfil operator overloading requirements and to demonstrate multi-attribute class design.

**Why we removed them:**  
Both features added **disconnected complexity** — logic that was not integrated into any other part of the system and therefore carried maintenance cost without delivering observable behaviour.

`clubPoints` in `Customer` was accumulated but never consumed. There was no discount logic, no redemption system, and no interaction between club points and ticket pricing. The field existed only to be printed. In a system where `Ticket` no longer carries a price (see below), club points have no meaningful role whatsoever.

`calcFinalPrice()` and `operator>` in `Ticket` depended on hardcoded price constants that varied by ticket type. Removing price logic is consistent with Deviation 2: since the system now derives ticket type automatically from the hall, there is no user-facing pricing step, and comparing ticket prices via `operator>` serves no purpose within the menu-driven flow. Keeping dead code with no call sites reduces readability and creates misleading impressions about the system's capabilities.

**What we changed:**  
We removed `clubPoints` and all associated methods from `Customer`, simplifying the class to a named guest with a visit counter. We removed `BASE_PRICE`, `THREE_D_SURCHARGE`, `VIP_SURCHARGE`, `MEAL_PRICE`, `calcFinalPrice()`, and `operator>` from the `Ticket` hierarchy. In their place, `printTicket()` (virtual, overridden in `VIPTicket`) outputs the screening details — movie title, hall number, date, 3D and meal flags — providing meaningful output without fabricated pricing arithmetic.

---

### Deviation 6 — Strict Memory Management, `const` Correctness, and Rule of Three

| | |
|---|---|
| **Status** | ✦ Required |
| **Files affected** | All `.cpp` and `.h` files |

**What the original authors likely did:**  
Based on the design documents, the original authors prioritised getting the system to compile and run over enforcing strict memory safety. Common patterns in student C++ code that we anticipated and corrected include: missing destructors on classes with `char*` or pointer-array members; copy constructors that perform shallow copies (aliasing `char*` names instead of duplicating them); missing `const` qualifiers on getter methods; and `operator=` implementations without self-assignment guards.

**Why these are Required fixes, not stylistic preferences:**  
These are not matters of code style. They are **correctness requirements** under the C++ standard.

**Memory management — the Rule of Three:**  
Any class that manages a dynamically allocated resource must provide all three of: a destructor, a copy constructor, and a copy assignment operator. Omitting any one of them causes the compiler to synthesise a default version that performs a *shallow copy*, meaning two objects end up sharing ownership of the same heap allocation. When either object is destroyed, the other is left holding a dangling pointer. The next access to that pointer is undefined behaviour.

We implemented the Rule of Three in full across all classes that own dynamic memory: `Person` (`char* name`), `Movie` (`char* title`), `Reviewer` (`char* publicationName`), `Guest` (`Ticket** tickets`), and `Cinema` (six separate dynamic pointer arrays). In each case, copy constructors perform deep copies using `new`/`strcpy` sequences with allocate-before-delete ordering (to maintain exception safety), and destructors release every allocation in reverse dependency order.

**Destructor ordering in `Cinema`:**  
Because objects hold non-owning *references* to other objects managed by `Cinema`, the destructor must release arrays in strict dependency order to avoid accessing freed memory:

```
shifts → employees   (Shift holds const Employee&)
guests → screenings  (Ticket holds const Screening&)
screenings → movies, halls  (Screening holds const Movie& and const Hall&)
halls → (base)
movies → (base)
```

Releasing `screenings` before `guests`, for example, would leave every `Ticket` inside every `Guest` holding a dangling `const Screening&`.

**`const` correctness:**  
Every getter method and every print method is declared `const`, preventing them from being called on `const`-qualified object references and ensuring they cannot accidentally modify object state. Method parameters that accept objects by reference use `const T&` consistently. This is enforced throughout the hierarchy, including across virtual overrides, where a `const` qualifier on the base class declaration is inherited by all overrides.

**Polymorphic cloning:**  
Because `Cinema` stores heterogeneous arrays of `Guest*` and `Hall*`, the copy constructor cannot call `new Guest(*src)` — it would slice the object to the base type. We implemented a virtual `clone()` method on both `Guest` and `Hall` hierarchies, returning `new T(*this)` from each concrete subclass. `Cinema`'s copy constructor calls `guests[i]->clone()` and `halls[i]->clone()`, preserving the concrete type (`Customer`, `Reviewer`, `Hall3D`, `Hall3DVIP`, etc.) through the copy.

---

## Deviation Summary Table

| # | Deviation | Classification | Primary Files Changed |
|---|---|---|---|
| 1 | Separate Movie/Hall; introduce `Screening` entity | **Required** | `Movie.h`, `Hall.h`, `Screening.h`, `Cinema.h/cpp` |
| 2 | Automated ticket type from Screening/Hall context | **Recommended** | `Cinema.h/cpp`, `main.cpp` |
| 3 | Dynamic 3D seat cap; per-screening seat tracking | **Recommended** | `Hall.h`, `Hall3D.h`, `Screening.h/cpp`, `Cinema.cpp` |
| 4 | Auto-initialize `visitCount` to 0 in constructor | **Required** | `Guest.h/cpp`, `Customer.h/cpp`, `Reviewer.h/cpp` |
| 5 | Remove `clubPoints`, `calcFinalPrice()`, `operator>` | **Recommended** | `Customer.h/cpp`, `Ticket.h/cpp`, `VIPTicket.h/cpp` |
| 6 | Full Rule of Three, `const` correctness, clone pattern | **Required** | All `.h` and `.cpp` files |

---

*All changes are marked with `// DEVIATION FROM ORIGINAL SPEC:` or `// CHANGED:` inline comments in the source files for traceability.*
