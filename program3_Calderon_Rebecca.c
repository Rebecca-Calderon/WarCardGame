//Name:Rebecca Calderon
//Programming Assignment 3

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct card_s{
	int rank;    //number
	char * type; //type of card
	struct card_s * nextptr;
}card_t;

//function prototypes
void rules(); //display rules
int playRound(); //simulate round
card_t * openCardDeck(); //open the card deck and place into a linkedlist setup
card_t * insertBackSetup(card_t *node, char *name, int cardrank); //take card from orginial deck and place in back of linked list for setup of game
int empty(card_t * node); //check to see if linked list is empty
void cleanUp(card_t * head); //free memory to prevent memory leaks
int deckSize(card_t * head); //count number of nodes in the linked list
card_t * search(card_t * node, int spot); //search list for a specific spot in the card deck indexing is similar to array setup
card_t * copyCard(card_t * node); //make a deep copy of card
card_t * removeCard(card_t * node, int spot); //remove card from linkedlist
card_t * insertBackDeck(card_t *head, card_t *node); //place card at end of pile
int compareCard(card_t * cardp1, card_t * cardp2); //compare cards
card_t * moveCardBack(card_t *head); //place card at top of deck to the bottom of the deck

int main()
{
	int seed;
	printf("Enter Seed: ");
	scanf("%d", &seed);
	
	srand(seed); //seed set
	rules();
	
	int player; //1 or 2
	int result;
	
	printf("Would you like to be player 1 or 2?\n");
	printf("Enter 1 or 2: ");
	scanf("%d", &player); //choose player
	
	for(int game = 1; game <= 5; ++game) //simulate games
	{
		printf("Alright lets play game %d.\n", game);
		printf("Lets split the deck.\n");

		result = playRound(); //play game
		
		if((result == 1 && player == 1) || (result == 2 && player == 2)) //determine who won
			printf("You won game %d.\n", game);
		else
			printf("I won game %d.\n", game);
	}
	
	return 0;
}

void rules()
{
	printf("Welcome to the card game war!\n");
	printf("Here are the rules.\n");
	printf("You have a pile of cards and I have a pile of cards.\n");
	printf("We will each pull the top card off of our respective deck and compare them.\n");
	printf("The card with the highest number will win the round and take both cards.\n");
	printf("However if there is a tie, then we have to we have to place one card faced down and the next one faced up and compare the results.\n");
	printf("Winner of the tie, will grab all the cards out. However if it's a tie again, then we repeat the same action.\n");
	printf("Ready? Here we go!\n");
}

int playRound()
{
	card_t * deck = NULL;
	card_t * player1 = NULL;
	card_t * player2 = NULL;

	//opening card deck here and deck = head
	deck = openCardDeck();  
	if(deck == NULL) //defensive programming
	{
		printf("Malloc did not work");
		return 0;
	}
	//printf("\n--Deck size is: %d--\n", deckSize(deck));

	//SPLITING THE CARDS HERE
	int turn = 0; // do 0 instead of 1
	
	while(deckSize(deck) != 0 || deck != NULL)
	{
		int spot;
		spot = rand() % deckSize(deck);
		if(turn % 2 == 0)
		{
			card_t * deckSearch = search(deck, spot); //deckSearch = to the node spot is referring to
			card_t * copy = copyCard(deckSearch); //now we are creating a copy of that node as to not disturb the original linked list
			player1 = insertBackDeck(player1, copy); //we are inserting that copyed node into the back of players linked list
			deck = removeCard(deck, spot); //now we alter original linked list back getting rid of that node
		}
		else
		{
			card_t * deckSearch = search(deck, spot);
			//printf("\nDeck Search is this: %d", deckSearch -> rank);
			card_t * copy = copyCard(deckSearch);
			player2 = insertBackDeck(player2, copy);
			deck = removeCard(deck, spot);
	
		
		}
		turn++;
		
	}
	//WHILE EITHER PLAYERS HAVE 26 CARDS IN THEIR DECK, WE CONTINUE TO PLAY

	//decksize might not be changind
	//when does x change?
	while((deckSize(player1) != 0) && (deckSize(player2) !=0) )
	{
		int result;

		result = compareCard(player1, player2);
		
		//if player 1 wins:
		if(result == 1) 
		{	
			card_t * temp = player2;
			//so the linked list player1 will move its head pointer, temp to the back of the linked list
			player1 = moveCardBack(player1); //new head with old one in the back
			player1 = insertBackDeck(player1, player2); //head is player1 and card going to the back is the head of the losers linkedlist
			player2 = removeCard(player2, 0);

		}

		//if player 2 wins:
		if(result == 2)
		{
			card_t * temp = player1;
			// head goes to the back of the list and then insert what the other player lost
			player2 = moveCardBack(player2); //so the linked list player1 will move its head pointer, temp to the back of the linked list
			player2 = insertBackDeck(player2, player1); 
			player1 = removeCard(player1, 0);
		}
		
		//if there is a tie, INITIATING WAR HERE
		if(result == 0) 
		{
			printf("We have a tie! This means war!!!\n");
			int flag = 0;
			card_t * tracker = NULL; // put all plays into this deck and then insertback while != 0 

			//in a tie scenario, put player 1's and player's cards into this linked list, result of the war, keep adding those same cards into this linked list until someone
			//wins, empty the cards into winners deck 

			//when decksize is bigger than 3
			
			while((deckSize(player1) >=3)&& (deckSize(player2) >= 3))
			{
				int winningcard;
				int result = compareCard(player1, player2);
				winningcard = compareCard(player1->nextptr->nextptr, player2->nextptr->nextptr); //so we skip over the heads nextptr to the next one
				if(winningcard == 1)
				{
					printf("Player 1 has won the war\n");
					//decktracker = player1->nextptr->nextptr
					//so the linked list player1 will move its head pointer, temp to the back of the linked list
					//moving the head, second and third card drawn for player 1 into the back of the linked list
					tracker = insertBackDeck(tracker, copyCard(player1));
					player1 = moveCardBack(player1);
					tracker = insertBackDeck(tracker, copyCard(player1));
					player1 = moveCardBack(player1);
					tracker = insertBackDeck(tracker, copyCard(player1));
					player1 = moveCardBack(player1);

					tracker = insertBackDeck(tracker, copyCard(player2));
					player1 = insertBackDeck(player1, player2); //will insert head of player 2 into back of player 1's deck
					player2 = removeCard(player2, 0);
					tracker = insertBackDeck(tracker, copyCard(player2));
					player1 = insertBackDeck(player1, player2); //same with player 2's nextptr
	
					player2 = removeCard(player2, 0);
					tracker = insertBackDeck(tracker, copyCard(player2));
					player1 = insertBackDeck(player1, player2);  // and its nextptr, so you call this 3 time for the tie, facedown and face up card, 3 in total 
					player2 = removeCard(player2, 0);
					flag = 1;
					return 1;
				}
				else if(winningcard == 2)
				{
					printf("Player 2 has won the war!\n");
					//moving player 2's card that we're played into the back of its own deck

					tracker = insertBackDeck(tracker, copyCard(player2));
					player2 = moveCardBack(player2);
					tracker = insertBackDeck(tracker, copyCard(player2));
					player2 = moveCardBack(player2);
					tracker = insertBackDeck(tracker, copyCard(player2));
					player2 = moveCardBack(player2);

					//here we insert the head, nextptr, and nextptr into the winner's linked list & remove them 
					//from the losers linked list
					tracker = insertBackDeck(tracker, copyCard(player1));
					player2 = insertBackDeck(player2, player1); 
					player1 = removeCard(player1, 0);

					tracker = insertBackDeck(tracker, copyCard(player1));
					player2 = insertBackDeck(player2, player1);
					player1 = removeCard(player1, 0);

					tracker = insertBackDeck(tracker, copyCard(player1));
					player2 = insertBackDeck(player2, player1);  
					player1 = removeCard(player1, 0);
					return 2;
				}
				else //we have another tie!!
				{
					printf("Another tie, war again:");
					int winningcard2;
					while((deckSize(player1) >=3) && (deckSize(player2) >= 3))
					{
					winningcard2 = compareCard(player1-> nextptr-> nextptr, player2 -> nextptr -> nextptr);
						if(winningcard2 == 1)
						{
							printf("Player 1 won this round");
							tracker = insertBackDeck(tracker, copyCard(player1));
							player1 = moveCardBack(player1);
							tracker = insertBackDeck(tracker, copyCard(player1));
							player1 = moveCardBack(player1);
							tracker = insertBackDeck(tracker, copyCard(player1));
							player1 = moveCardBack(player1);

							tracker = insertBackDeck(tracker, copyCard(player2));
							player1 = insertBackDeck(player1, player2); 
							player2 = removeCard(player2, 0);

							tracker = insertBackDeck(tracker, copyCard(player2));
							player1 = insertBackDeck(player1, player2);
							player2 = removeCard(player2, 0);

							tracker = insertBackDeck(tracker, copyCard(player2));
							player1 = insertBackDeck(player1, player2);
							player2 = removeCard(player2, 0);

						return 1;
						}
						else if(winningcard2 == 2)
						{
							printf("player 2 won this round");
							tracker = insertBackDeck(tracker, copyCard(player2));
							player2 = moveCardBack(player2);
							tracker = insertBackDeck(tracker, copyCard(player2));
							player2 = moveCardBack(player2);
							tracker = insertBackDeck(tracker, copyCard(player2));
							player2 = moveCardBack(player2);

							//here we insert the head, nextptr, and nextptr into the winner's linked list & remove them 
							//from the losers linked list
							tracker = insertBackDeck(tracker, copyCard(player1));
							player2 = insertBackDeck(player2, player1); 
							player1 = removeCard(player1, 0);

							tracker = insertBackDeck(tracker, copyCard(player1));
							player2 = insertBackDeck(player2, player1);
							player1 = removeCard(player1, 0);

							tracker = insertBackDeck(tracker, copyCard(player1));
							player2 = insertBackDeck(player2, player1);  
							player1 = removeCard(player1, 0);

							return 2;
						}
						else
							printf("we have another tie!!");
							break;
					}

					break;
				}

				while((decksize(player1) <=3 || deckSize(player2) <= 3))
				{

					
					int winningcard3;
					winningcard3 = compareCard(player1->nextptr, player2->nextptr);
					while(deckSize(player1) || deckSize(player2) != 0 )
					{
						if(winningcard3 == 1)
						{
							printf("player1 won this round\n");
						
							return 1;
						}
						if(winningcard3 == 2)
						{
							printf("player2 won this round\n");
							return 1;
						}
					}
					if(deckSize(player1) == 0)
						printf("player 2 wins");
						return 2;
					if(deckSize(player2) == 0)
						printf("player 1 wins");
						return 1;

				}
			}

		}
	}
	return 0;
}
	

card_t * openCardDeck()
{
	FILE *fptr = fopen("deck.txt", "r");
	
	char *name = (char *) malloc(sizeof(char) * 20);
	
	if (name == NULL)
	{
		printf("Error in malloc...\n");
		exit(1);
	}
	
	card_t * head = NULL;
	
	int cardrank = 13;
	int tracker = 1;
	while(fscanf(fptr, "%s", name) == 1)
	{			
		strcat(name, " ");
		
		if(tracker % 5 == 1)
		{
			strcat(name, "of Clubs");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 2)
		{
			strcat(name, "of Diamonds");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 3)
		{
			strcat(name, "of Hearts");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 4)
		{
			strcat(name, "of Spades");
			head = insertBackSetup(head, name, cardrank);
			tracker = 0;
			--cardrank;
		}
		
		++tracker;
		
	}
	
	free(name);
	fclose(fptr);
	
	return head;
}


card_t * insertBackSetup(card_t *node, char *name, int cardrank)
{
	
    if(empty(node)) //check to see if list is empty
    {
		node = (card_t *) malloc(sizeof(card_t));
		
		if(empty(node)) //check to see if malloc worked
		{
			printf("Did not allocate head successfully...");
			printf("Program Terminating...\n");
			exit(1);
		}
		else //otherwise populate data of new head node
		{
			node->type = (char *) malloc(sizeof(char) * 20);
			
			if(node->type == NULL)
			{
				printf("Error with malloc...\n");
				exit(1);
			}
			
			strcpy(node->type, name);
			node->rank = cardrank;
			node->nextptr = NULL; //must make new tail nextptr NULL!!!
			
			return node;  //terminate
		}
      
    }
	
	//here we know that the list has at least one node
	
	card_t *head = node; //keep pointer to head since we will need to return this address
	
	while(node->nextptr != NULL) //traverse to tail
		node = node->nextptr;
	
	node->nextptr = (card_t *) malloc(sizeof(card_t)); 
	
	if(node->nextptr == NULL) //see if new tail was allocated successfully
	{
		printf("Did not allocate node successfully...");
		return head; //terminate if tail didn't get created
	}
	
	//populate new node
	node->nextptr->type = (char *) malloc(sizeof(char) * 20);
	
	if(node->nextptr->type == NULL)
	{
		printf("Error with malloc...\n");
		exit(1);
	}
	
	strcpy(node->nextptr->type, name);
	node->nextptr->rank = cardrank;
	node->nextptr->nextptr = NULL; //very important
			
	return head; //return head node since we need our starting point of the linked list
}


int empty(card_t * node)
{
	return (node == NULL); //return condition result
}


void cleanUp(card_t * head)
{
	free(head);
}


int deckSize(card_t * head) //count number of nodes in the linked list
{
    int count = 0;
    card_t * current = head; // "temp2"
    while(current != NULL) //while the head != NULL
    {
        count++; //adding to count to see how many cards in deck
        current = current -> nextptr; //keep moving along nodes until we reach the end
    }
	//printf("Count is %d\n", count);
    return count;

}


card_t * search(card_t * node, int spot) //search list for a specific spot in the card deck indexing is similar to array setup
{//same as a push function
	
	if(node == NULL)
	{
		return NULL;
	}
	int x = 0;
	while(x != spot) 
	{
		node = node -> nextptr;
		x++;
	}
	return node;
	
}


card_t * copyCard(card_t * node) //make a deep copy of card
{	
	if(node == NULL)//base case
	{
		return NULL;
	}
	else
	{
		
		card_t * newNode = (card_t *)malloc(sizeof(card_t));
		if(newNode == NULL)
		{
			printf("Memory not allocated");
			exit (1);
		}
		newNode -> type = (char *)malloc(sizeof(char)* 20);
		if(newNode -> type == NULL)
		{
			printf("Memory not allocated");
			exit (1);
		}
		strcpy(newNode -> type, node -> type);
	
		newNode -> rank = node -> rank;
		newNode -> nextptr = NULL;
		return newNode;	
	}
	
}


card_t * removeCard(card_t * node, int spot) //remove card from linkedlist
{	// node passed in is the head of the linked list
	card_t * temp = node; //head
	card_t * temp2 = search(node, spot);
	
	if(temp == temp2)
	{
		temp = temp -> nextptr;
		free(temp2 -> type);
		free(temp2);
		return temp;
	}
	
	while(node -> nextptr != temp2)
	{
		node = node -> nextptr;
	}
	node -> nextptr = temp2 -> nextptr;
	free(temp2 -> type);
	free(temp2);
	return temp;
}


card_t * insertBackDeck(card_t *head, card_t *node) //place card at end of pile
{
	//allocating space for the node
	card_t * last = head; //kind of like a temp pointer here
	if(empty(head))
	{
		return node;
	}
	while(last->nextptr != NULL)
	{
		//traversing through linked list to find the node with nextptr null
		last = last->nextptr;
	}
	last -> nextptr = node; 
	last -> nextptr -> nextptr = NULL;
	return head;
}


int compareCard(card_t * cardp1, card_t * cardp2) //compare cards
{
	//both cardp1 and cardp2 are the heads of the players linked list 
	while(cardp1 && cardp2 != NULL)

	{
		if(cardp1 -> rank > cardp2 -> rank)
		{	
			return 1; //cardp1 -> rank;
		}
		if(cardp1 -> rank < cardp2 -> rank)
		{
			return 2; 
		}
		if(cardp1 -> rank == cardp2 -> rank)
		{
			return 0;
		}
	}
	printf("Memory not allocated for compare card\n");
	//compare the rank of both cardp1 -> rank 
	return 0;
}


card_t * moveCardBack(card_t *head) //place card at top of deck to the bottom of the deck
{	
	card_t * first = head;
	card_t * last = head;
	
	if(head != NULL && head -> nextptr != NULL)
	{
		
		while(last -> nextptr != NULL)
		{
	
			last = last -> nextptr;
		}

		first = head -> nextptr;
		last -> nextptr = head;
		head -> nextptr = NULL;
	
	}

	return first;
	
}
