
import pickle
import numpy as np
from scipy import misc
import matplotlib.pyplot as plt
import math

def psnr(img1, img2):
    mse = np.mean( (img1 - img2) ** 2 )
    if mse == 0:
        return 100
    PIXEL_MAX = 255.0
    return print(20 * math.log10(PIXEL_MAX / math.sqrt(mse)))


n1=misc.imread('/Users/brinalbheda/Desktop/HW6/1.png')
n2=misc.imread('/Users/brinalbheda/Desktop/HW6/2.png')
n3=misc.imread('/Users/brinalbheda/Desktop/HW6/3.png')
n4=misc.imread('/Users/brinalbheda/Desktop/HW6/4.png')

fr=open('feat.pkl','rb')
fp=open('pca_params.pkl','rb')
data1=pickle.load(fr)
pca_params=pickle.load(fp)
feat_new=data1['feature']
fs=np.load('fs.npy')
feat_new=feat_new.reshape(fs[0],fs[1],fs[2],fs[3])
transformed=feat_new.reshape(16,16)
bias=pca_params['Layer_%d/bias'%1]
kernels=pca_params['Layer_%d/kernel'%1]
e=np.zeros((1, kernels.shape[0]))
e[0,0]=1
transformed+=bias*e
sample_patches_centered_w_bias=np.matmul(transformed,np.linalg.pinv(np.transpose(kernels)))
sample_patches_centered=sample_patches_centered_w_bias-np.sqrt(96)*bias
feature_expectation = pca_params['Layer_%d/feature_expectation'%1].astype(np.float32)
sample_patches = sample_patches_centered + feature_expectation
sample_patches = sample_patches.reshape(4,2,2,96)
sample_patches=sample_patches.reshape(4,2,2,1,1,4,4,6)

img=list()
for i in range(2):
    l=np.dstack((sample_patches[:,i,0,0,0,:,:,:],sample_patches[:,i,1,0,0,:,:,:]))
    img.append(l)
sample_patches=np.hstack((img[0], img[1]))


transformed0=sample_patches.reshape(256,6)
kernels0=pca_params['Layer_%d/kernel'%0]
sample_patches_centered0=np.matmul(transformed0,np.linalg.pinv(np.transpose(kernels0)))
feature_expectation0 = pca_params['Layer_%d/feature_expectation'%0].astype(np.float32)
sample_patches0 = sample_patches_centered0 + feature_expectation0
sample_patches0 = sample_patches0.reshape(4,8,8,16)
sample_patches0=sample_patches0.reshape(4,8,8,1,1,4,4,1)

img=list()
image_h = np.zeros((8,4,4,32))
for i in range(8):
    l = np.dstack((sample_patches0[:,i,0,0,0,:,:,0],sample_patches0[:,i,1,0,0,:,:,0],sample_patches0[:,i,2,0,0,:,:,0],sample_patches0[:,i,3,0,0,:,:,0],sample_patches0[:,i,4,0,0,:,:,0],sample_patches0[:,i,5,0,0,:,:,0],sample_patches0[:,i,6,0,0,:,:,0],sample_patches0[:,i,7,0,0,:,:,0]))
    img.append(l)
imgo=np.hstack((img[0], img[1], img[2], img[3], img[4], img[5], img[6], img[7]))


plt.imsave('1r.png',imgo[0],cmap='gray')
plt.imsave('2r.png',imgo[1],cmap='gray')
plt.imsave('3r.png',imgo[2],cmap='gray')
plt.imsave('4r.png',imgo[3],cmap='gray')

psnr(n1,imgo[0])
psnr(n2,imgo[1])
psnr(n3,imgo[2])
psnr(n4,imgo[3])

