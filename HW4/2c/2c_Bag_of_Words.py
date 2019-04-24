#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 07 08:12:45 2019

@author: brinalbheda
"""

import cv2
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans

def readraw(file,ht,wd):
    A = np.fromfile(file, dtype='uint8', sep="")
    A = A.reshape([ht,wd])
    return A


path='/Users/brinalbheda/Desktop/DIP/HW4/2c/'
i_one=np.zeros((5,56,56))
i_zero=np.zeros((5,56,56))


for i in range(1,6):
    temp1=readraw(path+'one_'+str(i)+'.raw',28,28)
    temp1=cv2.resize(temp1,(0,0),fx=2,fy=2)
    i_one[i-1,:,:]=temp1
    temp0=readraw(path+'zero_'+str(i)+'.raw',28,28)
    temp0=cv2.resize(temp0,(0,0),fx=2,fy=2)
    i_zero[i-1,:,:]=temp0


plt.imshow(i_one[1,:,:],cmap='gray')
plt.imshow(i_zero[1,:,:],cmap='gray')

sift = cv2.xfeatures2d.SIFT_create()
keys1=list()
Des1=list()
keys0=list()
Des0=list()

for i in range(0,5):
    kp1,des1=sift.detectAndCompute(i_one[i,:,:].astype('uint8'),None)
    kp0,des0=sift.detectAndCompute(i_zero[i,:,:].astype('uint8'),None)
    Des1.append(des1)
    Des0.append(des0)

D0=np.concatenate(Des0)
D1=np.concatenate(Des1)

feature=np.concatenate((D0,D1))
###
kmeans = KMeans(n_clusters=2)
kmeans = kmeans.fit(feature)
labels = kmeans.predict(feature)
centroids = kmeans.cluster_centers_

eight=readraw(path+'eight.raw',28,28)
eight=cv2.resize(eight,(0,0),fx=2,fy=2)
kp8,des8=sift.detectAndCompute(eight,None)

dist0=np.linalg.norm(des8-centroids[0,:],axis=1)
dist1=np.linalg.norm(des8-centroids[1,:],axis=1)

labels=np.zeros((len(des8),1))
for i in range(0,len(des8)):
    if dist0[i]<dist1[i]:
        labels[i,0]=0
    if dist1[i]<dist0[i]:
        labels[i,0]=1

plt.hist(labels,bins=[0,0.01,0.99,1])

plt.show()

'''
plt.xlabel('Code Word')
plt.ylabel('Count')
plt.title('Histogram')
plt.show()
'''
