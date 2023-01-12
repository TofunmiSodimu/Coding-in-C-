#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;
int next;
int initial;
int start;
int cycle;
int my_index;
int cycle2;
int random2[MAX][MAX] = {0};

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int cycles(int p);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Update rank array
    for (int a = 0; a < candidate_count; a++)
    {
        if (strcmp(name, candidates[a]) == 0)
        {
            ranks[rank] = a;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int b = 0; b < candidate_count; b++)
    {
        for (int c = (b + 1); c < candidate_count; c++)
        {
            preferences[(ranks[b])][(ranks[c])]++;
        }
    }

    for (int p = 0; p < candidate_count; p++)
    {
        for (int q = 0; q < candidate_count; q++)
        {
            printf("%d = %d, %d\n", preferences[p][q], p, q);
        }
    }
    printf("\n");
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int d = 0; d < candidate_count; d++)
    {
        for (int e = 0; e < candidate_count; e++)
        {
            if ((preferences[d][e] > preferences[e][d]))
            {
                pairs[pair_count].winner = d;
                pairs[pair_count].loser = e;
                pair_count++;
            }
        }
    }
    printf("%d\n", pair_count);
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    int max = 0;
    int random [MAX][MAX];
    int win;
    int lose;

    // Copy preferences array into new array
    for (int f = 0; f < candidate_count; f++)
    {
        for (int g = 0; g < candidate_count; g++)
        {
            random[f][g] = preferences[f][g];
            //printf("%d", random[f][g]);
        }
    }

    // Order pairs based on strength of victory
    for (int j = 0; j < pair_count; j++)
    {
        for (int h = (candidate_count - 1); h >= 0; h--)
        {
            for (int i = (candidate_count - 1); i >= 0; i--)
            {
                if (random[h][i] >= max)
                {
                    max = random[h][i];
                    win = h;
                    lose = i;
                }
            }
        }

        pairs[j].winner = win;
        pairs[j].loser = lose;
        random[win][lose] = 0;
        max = 0;
        printf("%d, %d\n", pairs[j].winner, pairs[j].loser);
    }
    return;
}

// Create function that checks for cycles
int cycles(int p)
{
    for (int s = 0; s < candidate_count; s++)
    {
        my_index = 0;

        // Check if we have already tested and blocked off a path because it does not result in a cycle
        if (random2[next][s] != 2)
        {
            // Check if pair is locked and if we HAVE circled back to the start; return to locked function if so
            if ((locked[next][s] == 1) && (next == start))
            {
                return 11;
            }

            // Check if pair is locked and if we HAVE NOT circled back to the start; continue to follow path if so
            else if ((locked[next][s] == 1) && (next != start))
            {
                initial = next;
                next = s;
                my_index = 5;
                break;
            }

            // Check if pair is unlocked; move to next path if it is
            else if (locked[next][s] == 0)
            {
                random2[next][s] = 2;
                continue;
            }
        }
    }

    // Check if we are back to the first path, but still haven't found a cycle; if so, then return to main function
    if (next == initial)
    {
        return 13;
    }

    // If pathway exists, follow it
    if ((next != 11) && (next != 13))
    {
        if (my_index == 0)
        {
            if (initial != next)
            {
                random2[initial][next] = 2;
            }
            next = p;
        }

        // Keep calling the cycles function till we've searched for all cycles
        if ((cycles(p) == 11) || (cycles(p) == 13))
        {
            return cycles(p);
        }
    }
    return 10;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int k = 0; k < pair_count; k++)
    {
        locked[(pairs[k].winner)][(pairs[k].loser)] = true;
        start = pairs[k].winner;
        initial = pairs[k].loser;
        next = pairs[k].loser;

        // Reset random array to 0 for proper functioning
        for (int v = 0; v < candidate_count; v++)
        {
            for (int z = 0; z < candidate_count; z++)
            {
                random2[v][z] = 0;
            }
        }
        // Call function to check for cycle
        cycle = cycles(pairs[k].loser);

        // If cycle exists, unlock pair
        if (cycle == 11)
        {
            locked[(pairs[k].winner)][(pairs[k].loser)] = false;
            continue;
        }

        // If cycle doesn't exist, leave pair locked and move on to next pair
        else if (cycle == 13)
        {
            continue;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int win_not;
    for (int l = 0; l < candidate_count; l++)
    {
        for (int m = 0; m < candidate_count; m++)
        {
            for (int n = 0; n < candidate_count; n++)
            {
                if ((locked[l][m] == 1) && (locked[n][l] == 0))
                {
                    win_not = 1;
                }

                else
                {
                    win_not = 0;
                    break;
                }
            }

            if (win_not == 1)
            {
                printf("%s\n", candidates[l]);
                return;
            }
        }
    }
}