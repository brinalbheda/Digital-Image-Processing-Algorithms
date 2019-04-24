#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 05 18:19:22 2019

@author: brinalbheda
"""

import cv2
import numpy as np
from matplotlib import pyplot as plt
from numpy import inf
from numpy import array
from numpy.linalg import norm


img1 = cv2.imread('/Users/brinalbheda/Desktop/DIP/HW4/2b/river1.jpg')
gray1 = cv2.cvtColor(img1,cv2.COLOR_BGR2GRAY)

sift1 = cv2.xfeatures2d.SIFT_create()

kp1 = sift1.detect(gray1,None)
#type(kp1)

img_sift1 = cv2.drawKeypoints(gray1,kp1,img1)
cv2.imwrite('/Users/brinalbheda/Desktop/DIP/HW4/2b/sift_keypoints_river1.jpg',img_sift1)

#draw a circle with size of keypoint and it will even show its orientation
img1 = cv2.drawKeypoints(gray1,kp1,img1,flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
cv2.imwrite('/Users/brinalbheda/Desktop/DIP/HW4/2b/sift_keypoints_orient_river1.jpg',img1)

#computes the descriptors from the keypoints
kp1, des1 = sift1.compute(gray1,kp1)


img2 = cv2.imread('/Users/brinalbheda/Desktop/DIP/HW4/2b/river2.jpg')
gray2 = cv2.cvtColor(img2,cv2.COLOR_BGR2GRAY)

sift2 = cv2.xfeatures2d.SIFT_create()

kp2 = sift2.detect(gray2,None)

img_sift2 = cv2.drawKeypoints(gray2,kp2,img2)
cv2.imwrite('/Users/brinalbheda/Desktop/DIP/HW4/2b/sift_keypoints_river2.jpg',img_sift2)

#draw a circle with size of keypoint and it will even show its orientation
img2 = cv2.drawKeypoints(gray2,kp2,img2,flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
cv2.imwrite('/Users/brinalbheda/Desktop/DIP/HW4/2b/sift_keypoints_orient_river2.jpg',img2)

#computes the descriptors from the keypoints
kp2, des2 = sift2.compute(gray2,kp2)

'''
maxnorm = norm(des1, inf)
print(maxnorm)

ind = np.where(des1==maxnorm)

count = 0
for i in des1:
    for j in i:
        if (j == maxnorm):
            print(j)
            print(i)
            break
        count+=1
            
kp123 = kp1[count]

img_sift1l = cv2.drawKeypoints(gray1,kp123,img1)
cv2.imwrite('/Users/brinalbheda/Desktop/DIP_HW/HW4/2b/sift_large_keypoint_river1.jpg',img_sift1l)
'''

maxdes1 = 0

for i in des1:
    for j in i:
        maxdes1 = max(maxdes1,j)

print (maxdes1)

ind = np.where(des1==maxdes1)

'''
maxdes2 = 0

for i in des1:
    for j in i:
        maxdes2 = max(maxdes2,j)

print (maxdes2)
'''
#cv2.imwrite('/Users/brinalbheda/Desktop/DIP_HW/HW4/2b/sift_large_keypoint_river1.jpg',img_sift123)
'''
#Brute Force Matcher
# BFMatcher with default params
# create BFMatcher object
bf = cv2.BFMatcher()
matches = bf.knnMatch(des1,des2, k=2)

matches=sorted(matches, key=lambda x:x.distance)

# cv2.drawMatchesKnn expects list of lists as matches.
img3 = cv2.drawMatchesKnn(img1,kp1,img2,kp2,matches,None,flags=2)

cv2.imwrite('/Users/brinalbheda/Desktop/DIP_HW/HW4/2b/sift_match_keypoint_river.jpg',img3)

#plt.imshow(img3),plt.show()
'''

bf = cv2.BFMatcher()
matches=bf.match(des1,des2)
matches = sorted(matches, key = lambda x:x.distance)
img3= cv2.drawMatches(img1,kp1,img2,kp2,matches[:2],None)
cv2.imwrite('/Users/brinalbheda/Desktop/DIP/HW4/2b/sift_match_keypoint_river.jpg',img3)
plt.imshow(img3)
plt.show()
