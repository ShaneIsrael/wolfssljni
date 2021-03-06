/* WolfSSLContextTest.java
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

package com.wolfssl;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;
import static org.junit.Assert.*;

import com.wolfssl.WolfSSL;

public class WolfSSLContextTest {

    public final static int TEST_FAIL    = -1;
    public final static int TEST_SUCCESS =  0;

    public final static String cliCert = "./examples/certs/client-cert.pem";
    public final static String cliKey  = "./examples/certs/client-key.pem";
    public final static String caCert  = "./examples/certs/ca-cert.pem";
    public final static String bogusFile = "/dev/null";

    WolfSSLContext ctx;

    @Test
    public void testWolfSSLContext() throws WolfSSLException {

        System.out.println("WolfSSLContext Class");

        test_WolfSSLContext_new(WolfSSL.SSLv23_ServerMethod());
        test_WolfSSLContext_useCertificateFile();
        test_WolfSSLContext_usePrivateKeyFile();
        test_WolfSSLContext_loadVerifyLocations();
        test_WolfSSLContext_free();

    }

    public void test_WolfSSLContext_new(long method) {

        if (method != 0)
        {
            System.out.print("\tWolfSSLContext()");

            /* test failure case */
            try {

                ctx = new WolfSSLContext(0);

            } catch (WolfSSLException e) {

                /* now test success case */
                try {
                    ctx = new WolfSSLContext(method);
                } catch (WolfSSLException we) {
                    System.out.println("\t... failed");
                    fail("failed to create WolfSSLContext object");
                }

                System.out.println("\t... passed");
                return;
            }

            System.out.println("\t... failed");
            fail("failure case improperly succeeded, WolfSSLContext()");
        }
    }

    public void test_WolfSSLContext_useCertificateFile() {

        System.out.print("\tuseCertificateFile()");

        test_ucf(null, null, 9999, WolfSSL.SSL_FAILURE,
                 "useCertificateFile(null, null, 9999)");

        test_ucf(ctx, bogusFile, WolfSSL.SSL_FILETYPE_PEM, WolfSSL.SSL_FAILURE,
                 "useCertificateFile(ctx, bogusFile, SSL_FILETYPE_PEM)");

        test_ucf(ctx, cliCert, 9999, WolfSSL.SSL_FAILURE,
                 "useCertificateFile(ctx, cliCert, 9999)");

        test_ucf(ctx, cliCert, WolfSSL.SSL_FILETYPE_PEM,
                 WolfSSL.SSL_SUCCESS,
                 "useCertificateFile(ctx, cliCert, SSL_FILETYPE_PEM)");

        System.out.println("\t... passed");
    }

    /* helper for testing WolfSSLContext.useCertificateFile() */
    public void test_ucf(WolfSSLContext sslCtx, String filePath, int type,
                        int cond, String name) {

        int result;

        try {

            result = sslCtx.useCertificateFile(filePath, type);
            if (result != cond)
            {
                System.out.println("\t... failed");
                fail(name + " failed");
            }

        } catch (NullPointerException e) {

            /* correctly handle NULL pointer */
            if (sslCtx == null) {
                return;
            }
        }

        return;
    }

    public void test_WolfSSLContext_usePrivateKeyFile() {

        System.out.print("\tusePrivateKeyFile()");

        test_upkf(null, null, 9999, WolfSSL.SSL_FAILURE,
                 "usePrivateKeyFile(null, null, 9999)");

        test_upkf(ctx, bogusFile, WolfSSL.SSL_FILETYPE_PEM,
                  WolfSSL.SSL_FAILURE,
                 "usePrivateKeyFile(ctx, bogusFile, SSL_FILETYPE_PEM)");

        test_upkf(ctx, cliKey, 9999, WolfSSL.SSL_FAILURE,
                 "usePrivateKeyFile(ctx, cliKey, 9999)");

        test_upkf(ctx, cliKey, WolfSSL.SSL_FILETYPE_PEM, WolfSSL.SSL_SUCCESS,
                 "usePrivateKeyFile(ctx, cliKey, SSL_FILETYPE_PEM)");

        System.out.println("\t... passed");
    }

    /* helper for testing WolfSSLContext.usePrivateKeyFile() */
    public void test_upkf(WolfSSLContext sslCtx, String filePath, int type,
                        int cond, String name) {

        int result;

        try {

            result = sslCtx.usePrivateKeyFile(filePath, type);
            if (result != cond)
            {
                System.out.println("\t... failed");
                fail(name + " failed");
            }

        } catch (NullPointerException e) {

            /* correctly handle NULL pointer */
            if (sslCtx == null) {
                return;
            }
        }

        return;
    }

    public void test_WolfSSLContext_loadVerifyLocations() {

        System.out.print("\tloadVerifyLocations()");

        test_lvl(null, null, null, WolfSSL.SSL_FAILURE,
                "loadVerifyLocations(null, null, null)");

        test_lvl(ctx, null, null, WolfSSL.SSL_FAILURE,
                "loadVerifyLocations(ctx, null, null)");

        test_lvl(null, caCert, null, WolfSSL.SSL_FAILURE,
                "loadVerifyLocations(null, caCert, null)");

        test_lvl(ctx, caCert, null, WolfSSL.SSL_SUCCESS,
                "loadVerifyLocations(ctx, caCert, 0)");

        System.out.println("\t... passed");
    }

    /* helper for testing WolfSSLContext.loadVerifyLocations() */
    public void test_lvl(WolfSSLContext sslCtx, String filePath,
                         String dirPath, int cond, String name) {

        int result;

        try {

            result = sslCtx.loadVerifyLocations(filePath, dirPath);
            if (result != cond)
            {
                System.out.println("\t... failed");
                fail(name + " failed");
            }

        } catch (NullPointerException e) {

            /* correctly handle NULL pointer */
            if (sslCtx == null) {
                return;
            }
        }

        return;
    }

    public void test_WolfSSLContext_free() {

        System.out.print("\tfree()");
        ctx.free();
        System.out.println("\t\t\t... passed");
    }
}

