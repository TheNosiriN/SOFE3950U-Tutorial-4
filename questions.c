/*
* Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
*
* Copyright (C) 2015, Chinomso Nosiri (100846639), Ayush Patel (100823602), Bilal Khalil (100824635)
* All rights reserved.
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "questions.h"

question questions[NUM_QUESTIONS];

// Initializes the array of questions for the game
void initialize_game(void)
{
    // initialize each question struct and assign it to the questions array
    memset(questions, 0, sizeof(questions));

    questions[0] = (question){"programming", "What is the name of the data structure that allows adding elements from both ends?", "Deque", 100, false};
    questions[1] = (question){"programming", "What is the primary sorting algorithm used by the C++ standard library sort function?", "IntroSort", 200, false};
    questions[2] = (question){"programming", "What is the most common memory management model used in modern programming languages?", "Garbage collection", 300, false};
    questions[3] = (question){"programming", "What is the name of the standard that defines the C++ programming language?", "ISO/IEC 14882", 400, false};

    questions[4] = (question){"algorithms", "What is the name of the algorithm used to efficiently find the nth Fibonacci number?", "Matrix Exponentiation", 100, false};
    questions[5] = (question){"algorithms", "What is the time complexity of the best-known algorithms for finding the median of an unsorted array?", "O(n)", 200, false};
    questions[6] = (question){"algorithms", "What is the name of the algorithm used to find the strongly connected components in a directed graph?", "Kosaraju's algorithm", 300, false};
    questions[7] = (question){"algorithms", "What is the computational complexity class that represents decision problems that can be solved by a deterministic Turing machine in polynomial time?", "P", 400, false};

    questions[8] = (question){"databases", "What is the name of the algorithm used to ensure database normalization?", "Boyce-Codd Normal Form", 100, false};
    questions[9] = (question){"databases", "What is the name of the SQL feature used to enforce data integrity by automatically enforcing a set of constraints in a database?", "Triggers", 200, false};
    questions[10] = (question){"databases", "What is the term used to describe the phenomenon when one transaction reads data that is being modified by another transaction before it commits?", "Phantom Read", 300, false};
    questions[11] = (question){"databases", "What is the name of the database model that represents data as a collection of key-value pairs, suitable for hierarchical data storage?", "NoSQL", 400, false};
}

// Displays each of the remaining categories and question dollar values that have not been answered
void display_categories(void)
{
    printf("These are the list of questions left to answer:\n");

    // print categories and dollar values for each unanswered question in questions array
    int q_indices[NUM_QUESTIONS];
    for (int i=0, t=0; i<NUM_CATEGORIES; ++i){
        int num_questions = 0;

        for (int j=0; j<NUM_QUESTIONS; ++j){
            question* q = questions + j;
            if (!q->question || q->answered || strcmp(q->category, categories[i])!=0)continue;
            q_indices[num_questions++] = j;
        }
        if (!num_questions)continue;

        printf("%d. %s\n", ++t, categories[i]);
        for (int j=0; j<num_questions; ++j){
            printf("\t$%d -> %s\n", questions[q_indices[j]].value, questions[q_indices[j]].question);
        }
        printf("\n");
    }
}

// Displays the question for the index value
void display_question_index(int index)
{
    printf("For $%d!\n", questions[index].value);
    printf("%s\n", questions[index].question);
}

// Displays the question for the category and dollar value
void display_question(char *category, int value)
{
    for (int i=0; i<NUM_QUESTIONS; ++i){
        if (
            questions[i].value!=value ||
            strcmp(questions[i].category, category)!=0
        )continue;
        display_question_index(i);
        return;
    }
}

// Returns true if the answer is correct for the question for that index value
bool valid_answer_index(int index, char* answer){
    return strcmp(questions[index].answer, answer) == 0;
}

// Returns true if the answer is correct for the question for that category and dollar value
bool valid_answer(char *category, int value, char *answer)
{
    // Look into string comparison functions
    for (int i=0; i<NUM_QUESTIONS; ++i){
        if (
            questions[i].value!=value ||
            strcmp(questions[i].category, category)!=0
        )continue;
        return valid_answer_index(i, answer);
    }
    return false;
}

// Returns true if the question has already been answered
bool already_answered(char *category, int value)
{
    // lookup the question and see if it's already been marked as answered
    for (int i=0; i<NUM_QUESTIONS; ++i){
        if (
            questions[i].value!=value ||
            strcmp(questions[i].category, category)!=0
        )continue;
        return questions[i].answered;
    }
    return true;
}

// Returns the array index of the question for that category and dollar value
int get_question_index(char* category, int value)
{
    for (int i=0; i<NUM_QUESTIONS; ++i){
        if (
            questions[i].value!=value ||
            strcmp(questions[i].category, category)!=0
        )continue;
        return i;
    }
    return -1;
}
