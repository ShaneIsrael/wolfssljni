/* com_wolfssl_wolfcrypt_RSA.c
 *
 * Copyright (C) 2006-2013 wolfSSL Inc.
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <cyassl/options.h>
#include <cyassl/ctaocrypt/rsa.h>
#include "com_wolfssl_wolfcrypt_RSA.h"
#include <stdio.h>

JNIEXPORT jint JNICALL Java_com_wolfssl_wolfcrypt_RSA_doSign
  (JNIEnv* jenv, jobject jcl, jobject in, jlong inSz, jobject out,
   jintArray outSz, jobject keyDer, jlong keySz)
{
    int     ret;
    RNG     rng;
    RsaKey  myKey;
    unsigned int idx;
    unsigned int tmpOut;

    /* check in and key sz */
    if (((unsigned int)inSz  < 0) || 
        ((unsigned int)keySz < 0)) {
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
   
    /* get output buffer size */ 
    (*jenv)->GetIntArrayRegion(jenv, outSz, 0, 1, (jint*)&tmpOut);

    InitRng(&rng);
    InitRsaKey(&myKey, NULL);

    idx = 0;

    ret = RsaPrivateKeyDecode(keyBuf, &idx, &myKey, (unsigned int)keySz);
    if (ret == 0) {
        ret = RsaSSL_Sign(inBuf, (unsigned int)inSz, outBuf, tmpOut,
                &myKey, &rng);
        if (ret > 0) {
            /* save and convert to 0 for success */
            (*jenv)->SetIntArrayRegion(jenv, outSz, 0, 1, (jint*)&tmpOut);
            ret = 0;
        }
    } else {
        printf("RsaPrivateKeyDecode failed, ret = %d\n", ret);
    }

    FreeRsaKey(&myKey);

    return ret;
}

JNIEXPORT jint JNICALL Java_com_wolfssl_wolfcrypt_RSA_doVerify
  (JNIEnv* jenv, jobject jcl, jobject sig, jlong sigSz, jobject out,
   jlong outSz, jobject keyDer, jlong keySz)
{
    int     ret;
    RsaKey  myKey;
    unsigned int idx;

    /* check in and key sz */
    if (((unsigned int)sigSz < 0) || 
        ((unsigned int)keySz < 0) ||
        ((unsigned int)outSz < 0)) {
        return -1;
    }

    /* get pointers to our buffers */
    unsigned char* sigBuf = (*jenv)->GetDirectBufferAddress(jenv, sig);
    if (sigBuf == NULL) {
        printf("problem getting sig buffer address\n");
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
   
    InitRsaKey(&myKey, NULL);
    idx = 0;

    ret = RsaPublicKeyDecode(keyBuf, &idx, &myKey, (unsigned int)keySz);
    if (ret == 0) {
        ret = RsaSSL_Verify(sigBuf, (unsigned int)sigSz, outBuf,
                (unsigned int)outSz, &myKey);
        if (ret < 0) {
            printf("RsaSSL_Verify failed, ret = %d\n", ret);
            return ret;
        }
    } else {
        printf("RsaPublicKeyDecode failed, ret = %d\n", ret);
    }

    FreeRsaKey(&myKey);

    return ret;
}

JNIEXPORT jint JNICALL Java_com_wolfssl_wolfcrypt_RSA_doEnc
  (JNIEnv* jenv, jobject jcl, jobject in, jlong inSz, jobject out,
   jintArray outSz, jobject keyDer, jlong keySz)
{
    int     ret;
    RsaKey  myKey;
    RNG     rng;
    unsigned int idx;
    unsigned int tmpOut;

    /* check in and key sz */
    if (((unsigned int)inSz  < 0) || 
        ((unsigned int)keySz < 0)) {
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
   
    /* get output buffer size */ 
    (*jenv)->GetIntArrayRegion(jenv, outSz, 0, 1, (jint*)&tmpOut);

    InitRng(&rng);
    InitRsaKey(&myKey, NULL);

    idx = 0;

    ret = RsaPublicKeyDecode(keyBuf, &idx, &myKey, (unsigned int)keySz);
    if (ret == 0) {
        ret = RsaPublicEncrypt(inBuf, (unsigned int)inSz, outBuf, tmpOut,
                &myKey, &rng);
        if (ret > 0) {
            /* save and convert to 0 for success */
            (*jenv)->SetIntArrayRegion(jenv, outSz, 0, 1, (jint*)&ret);
            ret = 0;
        }
    } else {
        printf("RsaPublicKeyDecode failed, ret = %d\n", ret);
    }

    FreeRsaKey(&myKey);

    return ret;
}

JNIEXPORT jint JNICALL Java_com_wolfssl_wolfcrypt_RSA_doDec
  (JNIEnv* jenv, jobject jcl, jobject in, jlong inSz, jobject out,
   jlong outSz, jobject keyDer, jlong keySz)
{
    int     ret;
    RsaKey  myKey;
    unsigned int idx;

    /* check in and key sz */
    if (((unsigned int)inSz < 0) || 
        ((unsigned int)keySz < 0) ||
        ((unsigned int)outSz < 0)) {
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
   
    InitRsaKey(&myKey, NULL);
    idx = 0;

    ret = RsaPrivateKeyDecode(keyBuf, &idx, &myKey, (unsigned int)keySz);
    if (ret == 0) {
        ret = RsaPrivateDecrypt(inBuf, (unsigned int)inSz, outBuf,
                (unsigned int)outSz, &myKey);
        if (ret < 0) {
            printf("RsaPrivateDecrypt failed, ret = %d\n", ret);
            return ret;
        }
    } else {
        printf("RsaPrivateKeyDecode failed, ret = %d\n", ret);
    }

    FreeRsaKey(&myKey);

    return ret;
}
