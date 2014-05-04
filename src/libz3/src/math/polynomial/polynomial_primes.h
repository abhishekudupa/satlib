/*++
Copyright (c) 2012 Microsoft Corporation

Module Name:

    polynomial_primes.h

Abstract:

    Some prime numbers for modular computations.

Author:

    Leonardo (leonardo) 2011-12-21

Notes:

--*/
#ifndef _POLYNOMIAL_PRIMES_H_
#define _POLYNOMIAL_PRIMES_H_

namespace polynomial {
#define NUM_SMALL_PRIMES 11
    const unsigned g_small_primes[NUM_SMALL_PRIMES] = {
        13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53
    };
    
#if 0
#define NUM_BIG_PRIMES 77
    const unsigned g_big_primes[NUM_BIG_PRIMES] = {
        16777259, 16777289, 16777291, 16777331, 16777333, 16777337, 16777381,
        16777421, 16777441, 16777447, 16777469, 16777499, 16777507, 16777531,
        16777571, 16777577, 16777597, 16777601, 16777619, 16777633, 16777639,
        16777643, 16777669, 16777679, 16777681, 16777699, 16777711, 16777721,
        16777723, 16777729, 16777751, 16777777, 16777781, 16777807, 16777811, 
        16777823, 16777829, 16777837, 16777853, 16777903, 16777907, 16777949, 
        16777967, 16777973, 16777987, 16777991, 16778009, 16778023, 16778071, 
        16778077, 16778089, 16778123, 16778129, 16778137, 16778147, 16778173, 
        16778227, 16778231, 16778291, 16778309, 16778357, 16778383, 16778401, 
        16778413, 16778429, 16778441, 16778449, 16778453, 16778497, 16778521, 
        16778537, 16778543, 16778549, 16778561, 16778603, 16778623, 16778627
    };
#else
#define NUM_BIG_PRIMES 231
    const unsigned g_big_primes[NUM_BIG_PRIMES] = {
        39103, 39107, 39113, 39119, 39133, 39139, 39157, 39161, 39163, 39181, 39191, 
        39199, 39209, 39217, 39227, 39229, 39233, 39239, 39241, 39251, 39293, 39301, 
        39313, 39317, 39323, 39341, 39343, 39359, 39367, 39371, 39373, 39383, 39397, 
        39409, 39419, 39439, 39443, 39451, 39461, 39499, 39503, 39509, 39511, 39521, 
        39541, 39551, 39563, 39569, 39581, 39607, 39619, 39623, 39631, 39659, 39667, 
        39671, 39679, 39703, 39709, 39719, 39727, 39733, 39749, 39761, 39769, 39779, 
        39791, 39799, 39821, 39827, 39829, 39839, 39841, 39847, 39857, 39863, 39869, 
        39877, 39883, 39887, 39901, 39929, 39937, 39953, 39971, 39979, 39983, 39989, 
        40009, 40013, 40031, 40037, 40039, 40063, 40087, 40093, 40099, 40111, 40123, 
        40127, 40129, 40151, 40153, 40163, 40169, 40177, 40189, 40193, 40213, 40231, 
        40237, 40241, 40253, 40277, 40283, 40289, 40343, 40351, 40357, 40361, 40387, 
        40423, 40427, 40429, 40433, 40459, 40471, 40483, 40487, 40493, 40499, 40507, 
        40519, 40529, 40531, 40543, 40559, 40577, 40583, 40591, 40597, 40609, 40627, 
        40637, 40639, 40693, 40697, 40699, 40709, 40739, 40751, 40759, 40763, 40771, 
        40787, 40801, 40813, 40819, 40823, 40829, 40841, 40847, 40849, 40853, 40867, 
        40879, 40883, 40897, 40903, 40927, 40933, 40939, 40949, 40961, 40973, 40993, 
        41011, 41017, 41023, 41039, 41047, 41051, 41057, 41077, 41081, 41113, 41117, 
        41131, 41141, 41143, 41149, 41161, 41177, 41179, 41183, 41189, 41201, 41203, 
        41213, 41221, 41227, 41231, 41233, 41243, 41257, 41263, 41269, 41281, 41299, 
        41333, 41341, 41351, 41357, 41381, 41387, 41389, 41399, 41411, 41413, 41443, 
        41453, 41467, 41479, 41491, 41507, 41513, 41519, 41521, 41539, 41543, 41549
    };
#endif

};

#endif
