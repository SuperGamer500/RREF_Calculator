#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void PrintSystem(float equations[][99], int rowNumber, int collumnNumber)
{
    for (int i = 0; i < rowNumber; i++)
    {
        printf("equation [%d] [", i + 1);
        for (int j = 0; j < collumnNumber; j++)
        {
            if (fabsf(equations[i][j]) < 0.00001f)
            {
                printf("0");
            }
            else
            {
                printf("%.3f", equations[i][j]);
            }
            if (j != collumnNumber - 1)
            {
                printf(", ");
            }
        }
        printf("]\n");
    }
    printf("---------------\n\n");
}

void ReduceSystem(float equation[99], int index, int collumnAmount)
{
    if (index < 0 || index >= collumnAmount)
    {
        return;
    }
    if (equation[index] == 0)
    {
        return;
    }

    float reductionMultiplier = pow(equation[index], -1);

    for (int i = 0; i < collumnAmount; i++)
    {
        equation[i] *= reductionMultiplier;
    }
}

void Transverse(float equation1[99], float equation2[99], int size)
{
    float *storage = malloc(sizeof(float) * size);

    for (int i = 0; i < size; i++)
    {
        storage[i] = equation1[i];
    }

    for (int i = 0; i < size; i++)
    {
        equation1[i] = equation2[i];
        equation2[i] = storage[i];
    }

    free(storage);
}

void PickCollumn(float equations[][99], int collumn, int requiredRow, int *success, int rowNumber, int collumnAmount)
{
    *success = 1;
    for (int i = requiredRow; i < rowNumber; i++)
    {
        if (equations[i][collumn] != 0)
        {
            ReduceSystem(equations[i], collumn, collumnAmount);
            Transverse(equations[i], equations[requiredRow], collumnAmount);
            *success = 0;
            break;
        }
    }

    return;
}

void EchelonForm(float equations[][99], int rowNumber, int collumnAmount)
{
    int collumnToUse = -1;
    int currentCollumn = 0;
    int currentRow = 0;
    int length = 3;
    int success = 0;
    for (int i = 0; i < collumnAmount; i++)
    {
        PickCollumn(equations, i, currentRow, &success, rowNumber, collumnAmount);
        if (success == 1)
        {
            continue;
        }

        for (int j = 0; j < rowNumber; j++)
        {
            if (j != currentRow && equations[j][i] != 0)
            {
                float multiplier = -equations[j][i];

                for (int k = 0; k < collumnAmount; k++)
                {
                    equations[j][k] += equations[currentRow][k] * multiplier;
                }
            }
        }

        currentRow++;
        continue;
    }

    for (int i = 0; i < rowNumber; i++)
    {
        if (fabsf(equations[i][collumnAmount - 1]) > 0.0001f)
        {
            int nonZero = 1;
            for (int j = 0; j < collumnAmount - 1; j++)
            {
                if (fabsf(equations[i][j]) > 0.0001f)
                {
                    nonZero = 0;
                }
            }
            if (nonZero == 1)
            {
                printf("Inconsistent system\n");
                return;
            }
        }
        else
            continue;
    }

    printf("Consistent system\n");

    int cVar = 0;
    int basicVariables = 0;
    int freeVariables = 0;
    int checkRow = 0;
    while (cVar < collumnAmount - 1)
    {
        if (checkRow > rowNumber - 1)
        {
            printf("var %d is a free variable\n", cVar + 1);
            freeVariables++;
            cVar++;
            continue;
        }
        int found = 1;

        if (equations[checkRow][cVar] > 0.001f)
        {
            found = 0;
            if (fabsf(equations[checkRow][collumnAmount - 1]) > 0.001f)
                printf("var %d = %.3f", cVar + 1, equations[checkRow][collumnAmount - 1]);

            else
                printf("var %d = 0", cVar + 1);

            for (int g = cVar + 1; g < collumnAmount - 1; g++)
            {
                if (fabs(equations[checkRow][g]) > 0.001f)
                    printf(" + (%f)var %d", -equations[checkRow][g], g + 1);
            }
            printf("\n");
            checkRow++;
            basicVariables++;
        }

        if (found != 0)
        {
            printf("var %d = free variable\n", cVar + 1);
            freeVariables++;
        }
        cVar++;
        printf("\n");
    }
    printf("In total there are %d basic variables and %d free variables", basicVariables, freeVariables);
    printf("--------------------\n");
}

void DefaltEquationTable(float equations[][99], int rowAmount, int collumnAmount)
{
    for (int i = 0; i < rowAmount; i++)
    {
        for (int j = 0; j < collumnAmount; j++)
        {
            equations[i][j] = 0;
        }
    }
}

void ChangeRow(float equations[][99], int row, int collumnAmount)
{
    float numToUse = 0;
    for (int i = 0; i < collumnAmount; i++)
    {
        printf("Define collumn %d\n", i + 1);
        scanf("%f", &numToUse);
        equations[row][i] = numToUse;
        printf("Collumn %d is now %f\n", i + 1, numToUse);
        printf("---------------\n\n");
    }
}

int main()
{

    int amountOfRows = 0;
    int amountOfCollumns = 0;

    while (1 == 1)
    {
        printf("Please define how many rows present (must be less than 100 and greater than 1)\n");
        scanf("%d", &amountOfRows);
        if (amountOfRows >= 100 || amountOfRows <= 1)
        {
            printf("Please define a valid amount of rows\n");
        }
        else
            break;
    }

    while (1 == 1)
    {
        printf("Please define how many collumns present (must be less than 100 and greater than 1)\n");
        scanf("%d", &amountOfCollumns);
        if (amountOfCollumns >= 100 || amountOfCollumns <= 1)
        {
            printf("Please define a valid amount of collumns\n");
        }
        else
            break;
    }

    float equations[99][99];
    DefaltEquationTable(equations, amountOfRows, amountOfCollumns);
    int request = 2;
    while (1 == 1)
    {
        printf("Do you want to make any changes?\n1.Change row\n2.Stop\n");
        scanf("%d", &request);
        printf("-----------------\n\n");
        if (request == 1)
        {
            int row = 0;
            do
            {
                printf("Pick row (1-%d)\n", amountOfRows);
                scanf("%d", &row);
                if (row > amountOfRows || row < 1)
                    printf("Not a valid row\n");
                else
                    break;

                printf("-----------------\n\n");
            } while (1 == 1);
            printf("---------------\n\n");
            ChangeRow(equations, row + -1, amountOfCollumns);
        }
        else if (request == 2)
        {
            printf("---------------\n\n");
            break;
        }

        PrintSystem(equations, amountOfRows, amountOfCollumns);
    }

    EchelonForm(equations, amountOfRows, amountOfCollumns);

    return 0;
}
