/* com_wolfssl_wolfcrypt_ECC.c
 *
 * Copyright (C) 2006-2014 wolfSSL Inc.
 *
 * This file is part of CyaSSL.
 *
 * CyaSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * CyaSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef __ANDROID__
    #include <cyassl/options.h>
#endif

#include <cyassl/ctaocrypt/ecc.h>
#include <cyassl/ctaocrypt/asn.h>
#include "com_wolfssl_wolfcrypt_ECC.h"
#include <stdio.h>

JNIEXPORT jint JNICALL Java_com_wolfssl_wolfcrypt_ECC_doVerify
  (JNIEnv* jenv, jobject jcl, jobject sig, jlong sigSz, jobject hash,
   jlong hashSz, jobject keyDer, jlong keySz, jintArray result)
{
    int     ret;
    int     tmpResult;
    ecc_key myKey;

    if ((sigSz  < 0) || (hashSz < 0) || (keySz  < 0)) {
        return -1;
    }

    /* get pointers to our buffers */
    unsigned char* sigBuf = (*jenv)->GetDirectBufferAddress(jenv, sig);
    if (sigBuf == NULL) {
        printf("problem getting sig buffer address\n");
        return -1;
    }

    unsigned char* hashBuf = (*jenv)->GetDirectBufferAddress(jenv, hash);
    if (hashBuf == NULL) {
        printf("problem getting hash buffer address\n");
        return -1;
    }
    
    unsigned char* keyBuf = (*jenv)->GetDirectBufferAddress(jenv, keyDer);
    if (keyBuf == NULL) {
        printf("problem getting key buffer address\n");
        return -1;
    }
  
    ecc_init(&myKey);

    ret = ecc_import_x963(keyBuf, (unsigned int)keySz, &myKey);
 
    if (ret == 0) {
        ret = ecc_verify_hash(sigBuf, (unsigned int)sigSz, hashBuf,
                (unsigned int)hashSz, &tmpResult, &myKey);
        if (ret != 0) {
            printf("ecc_verify_hash failed, ret = %d\n", ret);
            ecc_free(&myKey);
            return -1;
        }
    } else {
        printf("ecc_import_x963 failed, ret = %d\n", ret);
        return -1;
    }

    ecc_free(&myKey);
   
    (*jenv)->SetIntArrayRegion(jenv, result, 0, 1, &tmpResult);
    
    return ret;
}

JNIEXPORT jint JNICALL Java_com_wolfssl_wolfcrypt_ECC_doSign
  (JNIEnv* jenv, jobject jcl, jobject in, jlong inSz, jobject out,
   jlongArray outSz, jobject keyDer, jlong keySz)
{
    int     ret;
    RNG     rng;
    ecc_key myKey;
    unsigned int tmpOut;
    unsigned int idx = 0;

    /* check in and key sz */
    if ((inSz  < 0) || (keySz < 0)) {
        return -1;
    }

    /* get pointers to our buffers */
    unsigned char* inBuf = (*jenv)->GetDirectBufferAddress(jenv, in);
    if (inBuf == NULL) {
        printf("problem getting in buffer address\n");
        return -1;
    }

    unsigned char* outBuf = (*jenv)->GetDirectBufferAddress(jenv, out);
    if (outBuf == NULL) {
        printf("problem getting out buffer address\n");
        return -1;
    }
    
    unsigned char* keyBuf = (*jenv)->GetDirectBufferAddress(jenv, keyDer);
    if (keyBuf == NULL) {
        printf("problem getting key buffer address\n");
        return -1;
    }
 
    InitRng(&rng);
    ecc_init(&myKey);

    ret = EccPrivateKeyDecode(keyBuf, &idx, &myKey, keySz);
    if (ret == 0) {
        ret = ecc_sign_hash(inBuf, (unsigned int)inSz, outBuf, &tmpOut,
                &rng, &myKey);
        if (ret != 0) {
            printf("ecc_sign_hash failed, ret = %d\n", ret);
            ecc_free(&myKey);
            return -1;
        }
    } else {
        printf("EccPrivateKeyDecode failed, ret = %d\n", ret);
        return -1;
    }

    ecc_free(&myKey);
    
    (*jenv)->SetLongArrayRegion(jenv, outSz, 0, 1, (jlong*)&tmpOut);

    return ret;
}

