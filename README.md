# non-zero-mutex
a mutex with non-zero default-, and implicit uninitialized-state, hereafter 'the idea'. i herewith consider the idea public and attributable to degski, the administrator, hereafter 'the admin', of https://github.com/degski/never-null-mutex , hereafter 'the repo'. the idea is particularly useful in *lockless* data structure designs.


## description
in (the art of) programming for concurrency, the use of atomic flags is mandated in order to implement the 'Waiting on an Element'-idiom (the idiom is non-optional). the idiom entails that a non-zero atomic_flag (constructed with value == true) indicates (atomically) **'construction-is-finalized'** of the specific object under construction, by virtue of being the last class member (in the definition order of the class).

due to its unlocked state being non-zero*, defining a ***a non-zero mutex*** as the last member of (an) object allows for that mutex to cameo as a **construction-finalized-flag** while the object is under construction. after construction, the mutex allows for fine-grained locking. the uninitialized state is implicit by not being any of the possible valid states, including **unlocked** (value == 1). a read-write-spinlock, can be designed at *zero-extra-cost* as compared to a write-spinlock using the same basic design. the idea can be qualified as both *an enhancement and an extension of the 'Waiting on an Element'-idiom*.


## implementation
an ***example*** implementation of the idea, a test-test-and-swap read-write spinlock, is implemented and [included in the repo](https://github.com/degski/non-zero-mutex/blob/3d93e754d13a58321ffd2ac90d638515b5696b86/include/non_zero_mutex.hpp#L39), hereafter 'the library'.


## licensing
the library licensing follows the ***logic*** of the Microsoft Visual Studio 2019 licensing Scheme. the library is licensed under a fixed dual licensing scheme.


### free license
**i herewith grant any entity that** ***qualifies*** **for a free Microsoft Visual Studio 2019 Community Edition License, the right to freely use this library subject to the [MIT License](https://choosealicense.com/licenses/mit/).**


### commercial license
**any entitiy that does not qualify for a free license is required to obtain a commercial license**.

* any entity that requires a commecial license, should contact the admin of the repo;
* the basis of the license-fee is last year's reported consolidated 12-month net turnover, everywhere 'the turnover';
* the annual license-fee, hereafter 'the fee', is calculated as **€0.015 of fee / €1'000.00 of turnover**;
* the fee is capped at €100'000;
* the fee is due in €;
* an implicit waiver is issued in case the annual fee does not surpass €100;
* the licensing scheme can not be modified;
* the commercial license will lapse to a free license after 20 years.

### other license
**send a proposal.**


## contact
degski@gmail.com
