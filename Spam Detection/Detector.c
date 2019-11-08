#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>



void create_test_and_train()
{
    FILE *file_pointer;
    file_pointer=fopen("english.txt","r");
    FILE *train_file_ptr, *test_file_ptr;

    train_file_ptr=fopen("train.txt","a+");
    test_file_ptr=fopen("test.txt","a+");

    int line_count=1;

    char c;

    while (line_count<=41)
    {
        c=getc(file_pointer);
        putc(c,train_file_ptr);
        if (c=='\n')
        {
            line_count++;
        }
    }

    while (line_count<=82)
    {
        c=getc(file_pointer);
        putc(c,test_file_ptr);
        if (c=='\n')
        {
            line_count++;
        }
    }

    while (line_count<=583)
    {
        c=getc(file_pointer);
        putc(c,train_file_ptr);
        if (c=='\n')
        {
            line_count++;
        }
    }

    while (line_count<=1084)
    {
        c=getc(file_pointer);
        putc(c,test_file_ptr);
        if  (c=='\n')
        {
            line_count++;
        }
    }

    fclose(file_pointer);
    fclose(train_file_ptr);
    fclose(test_file_ptr);
}


typedef struct sms_extract
{
    int histogram_of_sms[50];///carries the histogram of the sms
    char word[50][20];
    int number_of_words;
    int spam_number;///to hold the information whether an sms is spam or ham, its 1 if it was spam else 0 for ham
}var_line;

var_line train_line[542];///for train sms
var_line test_line[542];///for test sms



void store_words(FILE *file_pointer,struct sms_extract var_line[])
{
    int i,j,k;
    char c;

    for (i=0; i<541; i++)
    {
        c=getc(file_pointer);
        j=-1;


        while(c!='\n')
        {
            k=0;

            if (isalpha(c)>0)
            {
                j++;
	        }

            while (isalpha(c)>0)
            {
                var_line[i].word[j][k]=tolower(c);

                c=getc(file_pointer);
                k++;
            }

            if (c!='\n')
            {
                c=getc(file_pointer);
            }
        }

        if (var_line[i].word[j][0]=='s')
        {
            var_line[i].spam_number=1;///if its spam this array holds 1 else 0
        }
        else
        {
            var_line[i].spam_number=0;
        }

        var_line[i].number_of_words=j;///number of words including the word spam
    }
}


char ref_word[]="monkeyaround";

float lev_arr[50];///holds the value of average levhenstein distance of each cluster

typedef struct clutering_strings
{
    char cluster_string[10000][20];

    char representative_string[20];
}clustering;

clustering cluster_number[50];///creating 50 clusters

int word_index[50];///for no. of words in ith cluster

int return_distance (char word1[],char word2[])
{
    int len1,len2;
    len1=strlen(word1);
    len2=strlen(word2);

    int matrix[len1 + 1][len2 + 1];
    int i;
    for (i = 0; i <= len1; i++) {
        matrix[i][0] = i;
    }
    for (i = 0; i <= len2; i++) {
        matrix[0][i] = i;
    }
    for (i = 1; i <= len1; i++) {
        int j;
        char c1;

        c1 = word1[i-1];
        for (j = 1; j <= len2; j++) {
            char c2;

            c2 = word2[j-1];
            if (c1 == c2) {
                matrix[i][j] = matrix[i-1][j-1];
            }
            else {
                int delete;
                int insert;
                int substitute;
                int minimum;

                delete = matrix[i-1][j] + 1;
                insert = matrix[i][j-1] + 1;
                substitute = matrix[i-1][j-1] + 1;
                minimum = delete;
                if (insert < minimum) {
                    minimum = insert;
                }
                if (substitute < minimum) {
                    minimum = substitute;
                }
                matrix[i][j] = minimum;
            }
        }
    }
    return matrix[len1][len2];
}



float average_levhenstein_dist(int k)
{
    int j;
    float average,sum;

    for (j=0; j<=word_index[k]; j++)
    {
        sum=sum+return_distance(cluster_number[k].cluster_string[j],ref_word);
    }
    average=sum/(word_index[k]+1);

    return average;
}




void assign_representative_string(int k)
{
    int j,l,final_index;
    float dist,min_dist=100;

    for (j=0; j<=word_index[k]; j++)
    {
        l=return_distance(cluster_number[k].cluster_string[j],ref_word);
        dist=l-lev_arr[k];


        if (dist<0)
        {
            dist=dist*(-1);
        }

        if (min_dist>=dist)
        {
            min_dist=dist;
            final_index=j;
        }
    }

    strcpy(cluster_number[k].representative_string,cluster_number[k].cluster_string[final_index]);

}



void computekmeans(char string[],int n)
{
    int i,j=0,k;
    float l;
    int word_number;

    l=return_distance(string,ref_word);


    float length,nearest_length=100;

    for (i=0; i<n; i++)
    {

        length=return_distance(string,cluster_number[i].representative_string);

        if (nearest_length>=length)
        {
            nearest_length=length;
            k=i;
        }
    }

   word_number=word_index[k]+1;

    word_index[k]=word_number;




    strcpy(cluster_number[k].cluster_string[word_number],string);



    assign_representative_string(k);




}


void compute_histogram(struct sms_extract var_line[],int line_number)///line no. of file, i.e the index of the sms. This function computes the whole histogram array of the message passed
{
    int i,j;
    int nearest_dist;


    for (i=0; i<50; i++)
    {
        var_line[line_number].histogram_of_sms[i]=0;

        for (j=0; j<var_line[line_number].number_of_words; j++)
        {

          nearest_dist=return_distance(var_line[line_number].word[j],cluster_number[i].representative_string);

        var_line[line_number].histogram_of_sms[i]=var_line[line_number].histogram_of_sms[i]+nearest_dist;

        }

    }
}

int computeL1distance(int test_case_line_number, int train_case_line_number)
{
    int distance, i, total_distance=0;

    for (i=0; i<50; i++)
    {
        distance=test_line[test_case_line_number].histogram_of_sms[i]-train_line[train_case_line_number].histogram_of_sms[i];

        if (distance<0)
        {
            distance=distance*(-1);
        }

        total_distance=total_distance+distance;
    }

     return total_distance;
}

int main()
{
    create_test_and_train();

   FILE *fp;
   fp=fopen("train.txt","r");



   store_words(fp,train_line);
   int count=0;
   int random_line,random_word_number;

    int i,j;

    int n=50;///number of clusters

    for (i=0; i<n; i++)///initialising clustering
    {


        random_line=rand()%500;///random line
        random_word_number=rand()%train_line[random_line].number_of_words;///random word from the line

        strcpy(cluster_number[i].cluster_string[0] , train_line[random_line].word[random_word_number]);///putting 1 random string in each cluster
        strcpy(cluster_number[i].representative_string , cluster_number[i].cluster_string[0]);///at 1st step of compute k means the random strings will be our representative strings which are updated everytime when we call that funtion
        word_index[i]=0;

        lev_arr[i]=return_distance(cluster_number[i].cluster_string[0],ref_word);


    }

  for (i=0; i<541; i++)///applying k means to each word to find the final centres
   {
       for (j=0; j<train_line[i].number_of_words; j++)///number_of_words[i] is the number of words in the ith line of file
       {
           computekmeans(train_line[i].word[j],50);
           count++;
       }
   }

   printf("OUR REPRESENTATIVE STRINGS ARE :\n\n");
for (i=0; i<50; i++)
{
    printf("%d) %s\n",i+1,cluster_number[i].representative_string);
    ///printf("%f\n",lev_arr[i]);
}

fclose(fp);

remove("train.txt");

fp=fopen("test.txt","r");

store_words(fp,test_line);

///CALCULATING TRAIN HISTOGRAMS
for (i=0; i<541; i++)
{
    compute_histogram(train_line,i);
}


///CALCULATING TEST HISTOGRAMS
for (i=0; i<541; i++)
{
    compute_histogram(test_line,i);
}

///COMPARING L1 DISTANCES AND FINDING SPAM AND HAM
int spam=0,ham=0;
int dist, smallest_dist=10000, final_train_index;
int correct_detections=0;


for (i=0; i<541; i++)///index for test sms
{
    for (j=0; j<541; j++)///index for train sms
    {
        dist=computeL1distance(i,j);

        if (smallest_dist>=dist)
        {
            smallest_dist=dist;
            final_train_index=j;
        }

    }
     if (train_line[final_train_index].spam_number==1)
        {
            spam++;
            if (train_line[i].spam_number==1)
            {
                correct_detections++;
            }
        }
        else
        {
            ham++;
            if (train_line[i].spam_number==0)
            {
                correct_detections++;
            }
        }

}

printf("\n***********************************************\n");


printf("number of spam messages : %d\n",spam);
printf("number of ham messages : %d\n",ham);

printf("number of currect detections : %d\n",correct_detections);

float accuracy=correct_detections;

accuracy=(accuracy*100)/541;

printf("ACCURACY = %f percent \n",accuracy);

fclose(fp);

remove("test.txt");


 return 0;
}
