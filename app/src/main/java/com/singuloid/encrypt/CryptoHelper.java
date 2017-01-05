package com.singuloid.encrypt;


import android.os.ParcelFileDescriptor;

/**
 * Author: wangyun
 * Date: 17-1-5
 * Email: yun.wang@singuloid.com
 * LastUpdateTime: 17-1-5
 * LastUpdateBy: wangyun
 */
public class CryptoHelper {
	/**
	 * Encrypt byte[] passed in
	 * @param clearText which will be encrypted
	 * @param len the length of clearText
	 * @return the byte[] was encrypted
	 */
	public static native byte[] encryptData(byte[] clearText, int len);

	/**
	 * Decrypt byte[] passed in
	 * @param cipherText which will be decrypted
	 * @param len the length of cipherText
	 * @return the byte[] was decrypted
	 */
	public static native byte[] decryptData(byte[] cipherText, int len);

	/**
	 * Encrypt the file in srcPath and output encrypted file to desPath
	 * @param srcPath the absolute path of clear text file
	 * @param destPath the absolute path of cipher text file
	 */
	public static native void encryptFile(String srcPath, String destPath);

	/**
	 * Encrypt the file by the fd passed in
	 * @param fd the fd of clear text file
	 * @param destPath the absolute path of cipher text file
	 */
	public static native void encryptFd(int fd, String destPath);

	/**
	 * Encrypt the file by the ParcelFileDescriptor passed in
	 * @param srcFd the ParcelFileDescriptor of clear text file
	 * @param destPath the absolute path of cipher text file
	 */
	public static void encryptFile(ParcelFileDescriptor srcFd, String destPath) {
		int fd = srcFd.getFd();
		encryptFd(fd, destPath);
	}
	/**
	 * Decrypt the file in srcPath and output decrypted file to destPath
	 * @param srcPath the absolute path of cipher text file
	 * @param destPath the absolute path of clear text file
	 * @return the size of clear text file
	 */
	public static native int decryptFile(String srcPath, String destPath);

	/**
	 * Decrypt the file by the fd passed in
	 * @param fd the fd of cipher text file
	 * @param destPath the absolute path of clear text file
	 * @return  the size of clear text file
	 */
	public static native int decryptFd(int fd, String destPath);

	/**
	 * Decrypt the file by the ParcelFileDescriptor passed in
	 * @param srcFd the ParcelFileDescriptor of cipher text file
	 * @param destPath the absolute path of clear text file
	 * @return  the size of clear text file
	 */
	public static void decryptFile(ParcelFileDescriptor srcFd, String destPath) {
		int fd = srcFd.getFd();
		decryptFd(fd, destPath);
	}


}
