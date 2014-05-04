#if !defined __SATLIB_PTR_HASH_SET_HPP
#define __SATLIB_PTR_HASH_SET_HPP

#include "SatLibForwardDecls.hpp"
#include "HashSetConstants.hpp"
#include <iostream>

// For debugging
// #define __PTR_HASH_SET_DEBUG

namespace SatLib {

    /**
       A hash set for storing normal ptrs
       T must be a type of the form R* which has the following properties:
       3. R must implement the Hash() method
       4. R must implement the == operator
    */

    template<typename T> class PtrHashSet
    {
    private:
        u32 SeedIndex;
        u32 CurrentIndex;
        u64 NumElements;
        u32 NumBuckets;
        double RehashTrigger;
        vector<T>* Buckets;

        // Utility functions
        inline void Rehash();
        inline u32 GetHashIndex(u64 HashValue) const;

    public:
        // Constructor
        PtrHashSet(u32 InitialIndex = 0,
                   double RehashTrigger = DEFAULT_REHASH_TRIGGER);
        // Destructor
        ~PtrHashSet();
        // Insert
        inline T Add(T Element);
        // Delete
        inline bool Delete(T Element);
        // Membership
        // returns a null expression if none found
        inline T Contains(T Element) const;
        // Get the cached value for a ptr if it exists
        // delete the passed value
        // otherwise, cache it and return the passed argument
        inline T Get(T Element);
        // clear all entries
        inline void Clear();
        // Free pointers and clear
        inline void FreeAndClear();
        // Methods useful for debugging
        u32 GetNumBuckets() const;
        u32 GetNumElements() const;
        u32 GetCurrentIndex() const;
        double GetLoadFactor() const;
        double GetAverageChainLength() const;
        u32 GetMaxChainLength() const;
        u32 GetBucketsInUse() const;
    };

    // Implementation
    template<typename T> 
    PtrHashSet<T>::PtrHashSet(u32 InitialIndex, double RehashTrigger) 
        : SeedIndex(InitialIndex), CurrentIndex(InitialIndex),
          NumElements(0), NumBuckets(PrimeTable[InitialIndex]),
          RehashTrigger(RehashTrigger)
    {
        Buckets = new vector<T>[NumBuckets];
    }

    template<typename T>
    PtrHashSet<T>::~PtrHashSet()
    {
        for(u32 i = 0; i < NumBuckets; ++i) {
            Buckets[i].clear();
        }
        delete[] Buckets;
    }

    template<typename T>
    inline u32 PtrHashSet<T>::GetHashIndex(u64 HashValue) const
    {
        u64 Retval = HashValue % NumBuckets;
        // u64 Retval = ((HashValue ^ (HashValue >> (KeepBits[CurrentIndex]))) % NumBuckets);
        // assert(Retval < NumBuckets);
        return ((u32)Retval);
    }

    template<typename T>
    inline void PtrHashSet<T>::Rehash()
    {

#ifdef __PTR_HASH_SET_DEBUG

        cout << "-------------------------------------" << endl;
        cout << "Hash Table Debug: " << this << endl;
        cout << "Rehashing..." << endl;
        cout << "Old NumBuckets = " << NumBuckets << endl;
        cout << "NumElements = " << NumElements << endl;
        cout << "Load Factor = " << GetLoadFactor() << endl;
        cout << "Avg. Chain Length = " << GetAverageChainLength() << endl;
        cout << "Max. Chain Length = " << GetMaxChainLength() << endl;
        cout << "Buckets in Use = " << GetBucketsInUse() << endl;
        cout << "Hash Table Debug End" << endl;
        cout << "-------------------------------------" << endl;

#endif /* __PTR_HASH_SET_DEBUG */

        CurrentIndex++;
        u32 OldNumBuckets = NumBuckets;
        NumBuckets = PrimeTable[CurrentIndex];
        vector<T>* NewBuckets = new vector<T>[NumBuckets];
        for(u32 i = 0; i < OldNumBuckets; ++i) {
            u32 JLimit = Buckets[i].size();
            for(u32 j = 0; j < JLimit; ++j) {
                u64 HashValue = Buckets[i][j]->Hash();
                u64 NewHashIndex = GetHashIndex(HashValue);
                NewBuckets[NewHashIndex].push_back(Buckets[i][j]);
            }
            Buckets[i].clear();
        }
        delete[] Buckets;
        Buckets = NewBuckets;

#ifdef __PTR_HASH_SET_DEBUG

        cout << "-------------------------------------" << endl;
        cout << "Hash Table Debug: " << this << endl;
        cout << "After Rehashing: " << endl;
        cout << "New NumBuckets = " << NumBuckets << endl;
        cout << "NumElements = " << NumElements << endl;
        cout << "Load Factor = " << GetLoadFactor() << endl;
        cout << "Avg. Chain Length = " << GetAverageChainLength() << endl;
        cout << "Max. Chain Length = " << GetMaxChainLength() << endl;
        cout << "Buckets in Use = " << GetBucketsInUse() << endl;
        cout << "Hash Table Debug End" << endl;
        cout << "-------------------------------------" << endl;

#endif /* __PTR_HASH_SET_DEBUG */

    }

    template<typename T>
    inline T PtrHashSet<T>::Add(T Element)
    {
        if(((double)NumElements / (double)NumBuckets) >= RehashTrigger) {
            Rehash();
        }
        NumElements++;
        u64 HashIndex = GetHashIndex(Element->Hash());
        Buckets[HashIndex].push_back(Element);
        return Buckets[HashIndex].back();
    }

    template<typename T>
    inline bool PtrHashSet<T>::Delete(T Element)
    {
        u64 HashIndex = GetHashIndex(Element->Hash());
        typename vector<T>::iterator begin = Buckets[HashIndex].begin();
        typename vector<T>::iterator end = Buckets[HashIndex].end();
        bool Found = false;
        for(typename vector<T>::iterator it = begin; it != end; ++it) {
            if (*it == Element) {
                Found = true;
                Buckets[HashIndex].erase(it);
            }
        }
        if(Found) {
            NumElements--;
        }
        return Found;
    }

    template<typename T>
    inline T PtrHashSet<T>::Contains(T Element) const
    {
        u64 HashIndex = GetHashIndex(Element->Hash());
        u32 Limit = Buckets[HashIndex].size();
        for(u32 i = 0; i < Limit; ++i) {
            T CurrentElement = Buckets[HashIndex][i];
            if((*CurrentElement) == (*Element)) {
                return CurrentElement;
            }
        }
        return NULL;
    }

    template<typename T>
    inline T PtrHashSet<T>::Get(T Element)
    {
        T Cached;
        Cached = Contains(Element);
        if(Cached != NULL) {
            delete Element;
            return Cached;
        } else {
            Add(Element);
            return Element;
        }
    }

    template<typename T>
    inline void PtrHashSet<T>::Clear()
    {
        for(u32 i = 0; i < NumBuckets; ++i) {
            Buckets[i].clear();
        }
        delete[] Buckets;
        NumElements = 0;
        NumBuckets = PrimeTable[SeedIndex];
        CurrentIndex = SeedIndex;
        Buckets = new vector<T>[NumBuckets];
    }

    template<typename T>
        inline void PtrHashSet<T>::FreeAndClear()
    {

#ifdef __PTR_HASH_SET_DEBUG

        cout << "-------------------------------------" << endl;
        cout << "Hash Table Debug: " << this << endl;
        cout << "Final NumBuckets = " << NumBuckets << endl;
        cout << "Final NumElements = " << NumElements << endl;
        cout << "Final Load Factor = " << GetLoadFactor() << endl;
        cout << "Final Avg. Chain Length = " << GetAverageChainLength() << endl;
        cout << "Final Max. Chain Length = " << GetMaxChainLength() << endl;
        cout << "Final Buckets in Use = " << GetBucketsInUse() << endl;
        cout << "End Hash Table Debug" << endl;
        cout << "-------------------------------------" << endl;

#endif /* __PTR_HASH_SET_DEBUG */

        for(u32 i = 0; i < NumBuckets; ++i) {
            u32 BucketSize = Buckets[i].size();
            for(u32 j = 0; j < BucketSize; ++j) {
                delete Buckets[i][j];
            }
            Buckets[i].clear();
        }
        delete[] Buckets;
        NumElements = 0;
        NumBuckets = PrimeTable[SeedIndex];
        CurrentIndex = SeedIndex;
        Buckets = new vector<T>[NumBuckets];
    }

    template<typename T>
    u32 PtrHashSet<T>::GetNumBuckets() const
    {
        return NumBuckets;
    }

    template<typename T>
    u32 PtrHashSet<T>::GetNumElements() const
    {
        return NumElements;
    }

    template<typename T>
    u32 PtrHashSet<T>::GetCurrentIndex() const
    {
        return CurrentIndex;
    }

    template<typename T>
    double PtrHashSet<T>::GetLoadFactor() const
    {
        return ((double)NumElements / (double)NumBuckets);
    }

    template<typename T>
    double PtrHashSet<T>::GetAverageChainLength() const
    {
        double TotalChainLength = 0.0;
        u32 ActualBucketsInUse = 0;
        for(u32 i = 0; i < NumBuckets; ++i) {
            if(Buckets[i].size() == 0) {
                continue;
            }
            ActualBucketsInUse++;
            TotalChainLength += Buckets[i].size();
        }
        return (TotalChainLength / (double)ActualBucketsInUse);
    }

    template<typename T>
    u32 PtrHashSet<T>::GetMaxChainLength() const
    {
        u32 Max = 0;
        for(u32 i = 0; i < NumBuckets; ++i) {
            if(Buckets[i].size() > Max) {
                Max = Buckets[i].size();
            }
        }
        return Max;
    }
    
    template<typename T>
    u32 PtrHashSet<T>::GetBucketsInUse() const
    {
        u32 Retval = 0;
        for(u32 i = 0; i < NumBuckets; ++i) {
            if(Buckets[i].size() > 0) {
                Retval++;
            }
        }
        return Retval;
    }

} /* End namespace */

#endif /* __SATLIB_PTR_HASH_SET_HPP */

