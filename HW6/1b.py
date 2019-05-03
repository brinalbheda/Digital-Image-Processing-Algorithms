#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Mar  22 11:24:25 2019

@author: brinalbheda
"""

import time
import keras
from time import localtime, strftime
import numpy as np
import matplotlib.pyplot as plt
from keras.datasets import mnist
from tensorflow.examples.tutorials.mnist import input_data
from keras.models import Sequential
from keras.layers import Reshape, Flatten, Activation 
from keras.layers import Conv2D, MaxPooling2D, Dense
from keras import optimizers
from keras.optimizers import SGD, Adam
from keras.layers import Dropout
from sklearn.metrics import confusion_matrix


#split the dataset into the training and testing sets
(X_train, y_train), (X_test, y_test) = mnist.load_data()

#load the training data, validation data and testing data from MNIST dataset
#mnist_data = input_data.read_data_sets("MNIST_data/", one_hot=True, validation_size=0)

#load the training data, returns in numpy array form
#X_train = mnist_data.train.images 
X_train = np.reshape(X_train,(60000,28,28,1))
#y_train = np.asarray(mnist_data.train.labels, dtype=np.int32)
y_train = keras.utils.to_categorical(y_train, 10)

#load the validation data, returns in numpy array form
#X_valid = mnist_data.validation.images
#y_valid = np.asarray(mnist_data.validation.labels, dtype=np.int32)

#load the testing data, returns in numpy array form
#X_test = mnist_data.test.images 
X_test = (X_test.reshape(10000,28,28,1))
#y_test = np.asarray(mnist_data.test.labels, dtype=np.int32)
y_test = keras.utils.to_categorical(y_test, 10)

#define the LeNet5 architecture network model
def lenet5_architecture_model():
    #create a sequential model
    model = Sequential()
    #model.add(Reshape((28,28,1), input_shape=(784,)))
    model.add(Conv2D(6, kernel_size=(5, 5), strides=1, padding ='same', input_shape=(28,28,1), activation='relu', use_bias=True, kernel_initializer='random_uniform'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    #model.add(Dropout(0.5))
    model.add(Conv2D(16, kernel_size=(5, 5), strides=(1, 1), activation='relu', use_bias=True))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    #model.add(Dropout(0.5))
    model.add(Flatten())
    model.add(Dense(120, activation='relu'))
    model.add(Dense(84, activation='relu'))
    model.add(Dense(10, activation='softmax'))
    #define the optimizer and loss and metrics
    #sgd = optimizers.SGD(lr=0.001, decay=1e-6, momentum=0.9, nesterov=True)
    #compile the sequential model
    model.compile(optimizer='rmsprop', loss='categorical_crossentropy', metrics=['accuracy'])
    return model

#build the sequential model
lenet_model = lenet5_architecture_model()

#print the information of the model used above
print(lenet_model.summary())
print('Training the LeNet5 architecture model')

#train and fit the model
initial = time.time()
ts_value = localtime()
history = lenet_model.fit(X_train, y_train, validation_data=(X_test, y_test), epochs=10, batch_size=64, verbose=1) 
end = time.time()

print ("\nThe LeNet5 model takes %0.2f seconds to train\n"%(end - initial))

#list all data in history
print(history.history.keys())

#final evaluation of the lenet5 sequential model
score = lenet_model.evaluate(X_test, y_test, batch_size=64, verbose=1)    #first two elements can be X_train, y_train or X_test, y_test

#printing the error, loss for the model after the evaluation
print('LeNet5 architecture Error for testing: %.4f%%' % (100-score[1]*100))
print('LeNet5 architecture Loss for testing: %.4f%%' % score[0])

#the plot model history function plots the graphs showing loss and accuracy vs epoch/iteration
def plot_model_history(history):
    
    #summarize history for accuracy
    plt.plot(history.history['acc'])
    plt.plot(history.history['val_acc'])
    plt.legend(['Training', 'Testing'], loc='lower right')
    plt.ylabel('Accuracy')
    plt.xlabel('Epoch/Iteration')
    plt.title('Epoch-Accuracy plot for training and testing')
    plt.show()
    
plot_model_history(history)

#printing the accuracy for the model after the evaluation
print('LeNet5 architecture Accuracy for testing: %.4f%%' % (score[1]*100))


test_images = X_test[:2000]
test_labels = y_test[:2000]
predictions = lenet_model.predict(test_images.reshape(2000,28,28,1))
matrix = confusion_matrix(y_train.argmax(axis=1),predictions.argmax(axis=1))
matrix = 100*matrix/np.sum(matrix,axis=1)


