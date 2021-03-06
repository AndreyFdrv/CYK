#include <stdio.h>
#include <string.h>
extern void Step1(char *text);
extern bool isTerminal(char t);
extern void Step2(char *text);
extern bool isLeftBorder(char c);
extern bool isRightBorder(char c);
extern void substr(char *str1, char *str2, int begin, int end);
extern void begin_substr(char *str1, char *str2, int end);
extern void end_substr(char *str1, char *str2, int begin);
extern void Step3(char *text);
extern bool isRuleNew(char *text, char *rule, int right_part_begin);
extern void Step4(char *text);
extern void Step5(char *text);
int main(int argc, char *argv[])
{
	FILE* in=fopen(argv[1],"r");
	if(in==NULL)
		printf("File does not exist\n");
	else
	{
		char grammar[100] = "\0";
		char* str_err;
		char str[50];
		char word[20];
		fgets(word, 20, in);
		str_err = fgets(str, 50, in);
		while (str_err != NULL)
		{
			if (str[0] != 10)
			{
				strcat(grammar, str);
				str_err = fgets(str, 50, in);
			}
			else
				str_err = fgets(str, 50, in);
		}
		word[strlen(word) - 1] = '\0';
		Step1(grammar);
		Step2(grammar);
		Step3(grammar);
		Step4(grammar);
		Step5(grammar);
		printf("The grammar after normalization:\n");
		printf("%s\n", grammar);
		int N = strlen(word);
		bool mas[20][20][20];
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				for (int k = 0; k < 20; k++)
					mas[i][j][k] = false;
			}
		}
		for (int i = 0; i<N; i++)
		{
			char symbol[2];
			symbol[0]= word[i];
			symbol[1] = '\0';
			char cur_nonterminal, left_nonterminal = grammar[0];
			int nonterminal_index, left_nonoterminal_index = 0;
			for (int k = 2; grammar[k] != '\0';)
			{
				int left_border, right_border;
				if (grammar[k] == '\n')
				{
					left_nonterminal = grammar[k + 1];
					left_nonoterminal_index = k + 1;
				}
				if (isLeftBorder(grammar[k]))
				{
					left_border = k;
					k++;
					while (!isRightBorder(grammar[k]))
					{
						right_border = k + 1;
						k++;
					}
					char right_part[10];
					substr(right_part, grammar, left_border + 1, right_border - 1);
					if (!strcmp(right_part, symbol))
					{
						if (left_nonterminal=='S')
							mas[i][i][0] = true;
						else
							mas[i][i][left_nonterminal - 'A' + 1] = true;
					}
				}
				else
					k++;
			}
		}
		for (int diag = 1; diag < N; diag++)
		{
			for (int i = 0; i < N - diag; i++)
			{
				for (int l = i; l < i + diag; l++)
				{
					for (int k = 0; k < 20; k++)
					{
						if (mas[i][l][k])
						{
							for (int ik = 0; ik < 20; ik++)
							{
								if (mas[l+1][i + diag][ik])
								{
									char new_pair[3];
									if (k == 0)
										new_pair[0] = 'S';
									else
										new_pair[0] = 'A' + k - 1;
									if (ik == 0)
										new_pair[1] = 'S';
									else
										new_pair[1] = 'A' + ik - 1;
									new_pair[2] = '\0';
									char cur_nonterminal, left_nonterminal = 'S';
									int nonterminal_index, left_nonoterminal_index = 0;
									for (int m = 2; grammar[m] != '\0';)
									{
										int left_border, right_border;
										if (grammar[m] == '\n')
										{
											left_nonterminal = grammar[m + 1];
											left_nonoterminal_index = m + 1;
										}
										if (isLeftBorder(grammar[m]))
										{
											left_border = m;
											m++;
											while (!isRightBorder(grammar[m]))
											{
												right_border = m + 1;
												m++;
											}
											char right_part[10];
											substr(right_part, grammar, left_border + 1, right_border - 1);
											if (!strcmp(right_part, new_pair))
											{
												if (left_nonterminal == 'S')
													mas[i][i + diag][0] = true;
												else
													mas[i][i + diag][left_nonterminal - 'A' + 1] = true;
											}
										}
										else
											m++;
									}
								}
							}
						}
					}
				}
			}
		}
		if (mas[0][N - 1][0])
			printf("The grammar has the word %s, this word is correct\n", word);
		else
			printf("Word %s is incorrect\n", word);
		char old_str1[100] = "\0", old_str2[100] = "\0", old_str3[100] = "\0", old_str4[100] = "\0";
		for (int i = N - 1; i >= 0; i--)
		{
			for (int j = 0; j < N; j++)
			{
				char str1[100] = "|--", str2[100] = "|", str3[100] = "|", str4[100] = "|";
				int times = 0, old_k = 20;
				bool isFull = false;
				for (int k = 0; k < 20; k++)
				{
					if (times == 2)
					{
						old_k = k;
						break;
					}
					if (mas[i][j][k])
					{
						if (k == 0)
							strcat(str2, "S");
						else
						{
							char letter[2];
							letter[0] = 'A' + k - 1;
							letter[1] = '\0';
							strcat(str2, letter);
						}
						times++;
					}
				}
				if (times == 0)
					strcat(str2, "  ");
				else if (times == 1)
					strcat(str2, " ");
				else
					isFull = true;
				if (isFull)
				{
					times = 0, isFull=false;
					for (int k = old_k; k < 20; k++)
					{
						if (times == 2)
						{
							old_k = k;
							break;
						}
						if (mas[i][j][k])
						{
							char letter[2];
							letter[0] = 'A' + k - 1;
							letter[1] = '\0';
							strcat(str3, letter);
							times++;
						}
					}
					if (times == 0)
						strcat(str3, "  ");
					else if (times == 1)
						strcat(str3, " ");
					else
						isFull = true;
				}
				else
					strcat(str3, "  ");
				if (isFull)
				{
					times = 0;
					for (int k = old_k; k < 20; k++)
					{
						if (times == 2)
						{
							old_k = k;
							break;
						}
						if (mas[i][j][k])
						{
							char letter[2];
							letter[0] = 'A' + k - 1;
							letter[1] = '\0';
							strcat(str4, letter);
							times++;
						}
					}
					if (times == 0)
						strcat(str4, "  ");
					if (times == 1)
						strcat(str4, " ");
				}
				else
					strcat(str4, "  ");
				strcat(old_str1, str1);
				strcat(old_str2, str2);
				strcat(old_str3, str3);
				strcat(old_str4, str4);
			}
			strcat(old_str1, "|\n");
			strcat(old_str2, "|\n");
			strcat(old_str3, "|\n");
			strcat(old_str4, "|\n");
			printf("%s", old_str1);
			printf("%s", old_str2);
			printf("%s", old_str3);
			printf("%s", old_str4);
			old_str1[0]='\0';
			old_str2[0]='\0';
			old_str3[0]='\0';
			old_str4[0]='\0';
		}
		for (int i = 0; i < N; i++)
			printf("|--");
		printf("|\n");
		fclose(in);
	}
}
