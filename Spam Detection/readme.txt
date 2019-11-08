Basically what I have done in my code is that I created a structure  for storing messages whose variable are correspondingly for test and trains and one structure for having cluster of words including the representative word.
In this code I have divided all words into 50 clusters.

STRUCTURES AND VARIABBLES DEFINED :

1) sms_extract :
   it basically will contain the following information regardin a particular line (in each of the two corresponding variables train_line[] and test_line[])
   *histogram_of_sms[] -> array containingthe histogram of the sms(or line)
   *word[][] -> 2D string that carries all the words of the sms.
   *number_of_words -> number of the words in each line.
   *spam_number -> Its 0 when its ham and 1 when its spam.

2) lev_arr[] : 
   This array basically contains the average levhenstein distances of the centre of each cluster.

3) ref_word[] :
   This string contains our reference word.

4) clustering_strings :
   we have made only one variable cluster_number[] fromthis structure.
   It has 2 components
   *clustering_string[][] -> Its a 2D string which would contain all the strings of the cluster.
   *representative_string[] -> It is the word whose distance from reference word is the nearest to average of distance of each word of cluster from reference word.

5) word_index[] :
   Its a 2D array which contains the information of number of words in the i-th cluster.


FUNCTIONS DEFINED :

1) create_test_and_train :
   In this we basically open english,txt file in read mode and created test.txt and train.txt
   Now we read character by character from english.txt and stored sccordingly in train and test files such that each contains equal no. of spams and hams

2) store_words :
   In this 1st parameter is the file which needs to be stored and the next parameter is the variable of the sms_extract structure(either test_line or train_line).
   This function scan the corresponding file character by character and using 'isalpha' it stores words.

3) return_distance :
   This function takes 2 strings as its parameters and calculate the levhenstein distance.
   It calculates the length of both of the strings (say n1 and n2), construct a matrix of size (n1+1) * (n2+1)
   Now 1st row and 1t column are assigned 0 to n1 and o to n2 values
   Now using a nested loop we examine each character of both  words .
   Then we calculate the min of matrix[i-1][j] + 1 , matrix[i][j-1] + 1 and matrix[i-1][j-1] + 1 within the nested loop and assign matrix[i][j] its value
   This way once the loop ends martix[n1][n2] is our levhenstein distance.

4) average_levhenstein_distance  :
   Its called in the function computekmeans to update the lev_arr[] each time that function is called.
   It basically takes k as the parameter and its outpur is the average levenstein distance of the whole cluster from the reference word.
   
5) assign_representative_string :
   It also takes k as parameter and used in computekmeans to update the centre each time that function is called.
   It basically computes the distance of each of the string in k-th cluster and the string for which this distance is the nearest to lev_arr[k] is assigned as the representative string of that cluster.

6) computekmeans :
   This funtion is based on the kmeans algorithm. 
   Here we pass string and number of clusters as our argument and it compares its levhenstein distance with all the representative string and for the lowest distance its introduced into the corresponding cluster.
   And at last we update the average distance and centres.

7) compute_histogram :
   Two arguments are passed in it, variable of sms_extract structure and the line number(sms no.)
   It calculates the distance of each word from representative word and adds up into the correspponding array of histogram.This way it contains of the similarity between cluster centers and sms.

8) computeL1distance :
   As we will only be calculating the distance between one train sms and one test sms ata a time, so the parameters are the test sms no. and the train sms no.
   Using loop this function calculates difference of histograms and add upp the unsigned values to get L1 distance.


INSIDE THE MAIN() :

1st funtion create_test_and_train() is called to create test and train files

Then we open train file, store its wordsin the structure
using rand() function we give input of some random words in the clusters.
Now we give input of each word of those train sms to computekmeans to finally assign the representative words.
Then we close it.

Now we are printing the final representative strings.

Then we ope test file using the same pointer, and store its words also in the structure.

Now we have accessed both files, so we compute the histograms of all messages.

Then we compare L1 distances corresponding to each sms in test file  with each in train file to get the most similar sms.
Then we assign these sms as spam or ham according to the correspondin sms in train file.
And as we were already knowing which sms were spam and which were ham we calculted no. of correct detections on the basis of which accuracy is calculated at last.



How to run progamme
To open in ubuntu
1.Open terminal.
2.type "cd_location"
4.type "gcc -o filename.exe filename.c"
5.type "./a.out" 

 