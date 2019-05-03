import pickle
import numpy as np
import data
import saab
import keras
import sklearn
from scipy import misc


def main():
    # load data
    fr=open('pca_params.pkl','rb')  
    pca_params=pickle.load(fr)
    fr.close()
    
    # read data
    #train_images, train_labels, test_images, test_labels, class_list = data.import_data("0-9")
    n1 = misc.imread('/Users/brinalbheda/Desktop/HW6/1.png').reshape(32,32,1)
    n2 = misc.imread('/Users/brinalbheda/Desktop/HW6/2.png').reshape(32,32,1)
    n3 = misc.imread('/Users/brinalbheda/Desktop/HW6/3.png').reshape(32,32,1)
    n4 = misc.imread('/Users/brinalbheda/Desktop/HW6/4.png').reshape(32,32,1)
    arr_train_images = np.stack([n1,n2,n3,n4])
    arr_train_images = arr_train_images.astype('float32')
    train_images = arr_train_images
     
    print('Training image size:', train_images.shape)
    #print('Testing_image size:', test_images.shape)
	
    # Training
    print('--------Training--------')
    feature=saab.initialize(train_images, pca_params) 
    fs = feature.shape
    np.save('fs',fs)
    feature=feature.reshape(feature.shape[0],-1)
    print("S4 shape:", feature.shape)
    print('--------Finish Feature Extraction subnet--------')
    feat={}
    feat['feature']=feature
    	
    # save data
    fw=open('feat.pkl','wb')    
    pickle.dump(feat, fw)    
    fw.close()

if __name__ == '__main__':
	main()
