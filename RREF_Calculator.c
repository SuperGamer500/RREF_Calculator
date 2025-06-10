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

void EchelonForm(float equations[][99], int rowNumber, int collumnAmount, int *success)
{
    int collumnToUse = -1;
    int currentCollumn = 0;
    int currentRow = 0;
    int length = 3;
    *success = 0;
    for (int i = 0; i < collumnAmount; i++)
    {
        PickCollumn(equations, i, currentRow, success, rowNumber, collumnAmount);
        if (*success == 1)
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
                *success = 1;
                printf("Inconsistent system\n");
                printf("-----------------\n\n");
                return;
            }
        }
        else
            continue;
    }
    *success = 0;
    printf("Consistent system\n\n");
    printf("-----------------\n\n");
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

void PrintWeights(float equations[][99], int rowNumber, int collumnAmount, int *basicVariables, int *freeVariables)
{
    int cVar = 0;
    int checkRow = 0;
    while (cVar < collumnAmount - 1)
    {
        if (checkRow > rowNumber - 1)
        {
            printf("var %d is a free variable\n", cVar + 1);
            (*freeVariables)++;
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
            (*basicVariables)++;
        }

        if (found != 0)
        {
            printf("var %d = free variable\n", cVar + 1);
            (*freeVariables)++;
        }
        cVar++;
        printf("\n");
    }
    printf("In total there are %d basic variables and %d free variables\n", *basicVariables, *freeVariables);
    printf("--------------------\n");
}

void CalcResult(float equations[][99], int rowNumber, int collumnAmount, float *WeightsArray)
{
    int cVar = 0;
    int checkRow = 0;
    while (cVar < collumnAmount - 1)
    {
        if (checkRow > rowNumber - 1)
        {
            printf("var %d is a free variable\n", cVar + 1);
            printf("Please define test 1 var %d\n", cVar + 1);
            scanf("%f", &WeightsArray[cVar]);
            printf("--------------------\n");
            cVar++;
            continue;
        }
        int found = 1;

        if (equations[checkRow][cVar] > 0.001f)
        {
            found = 0;
            checkRow++;
        }

        if (found != 0)
        {
            printf("var %d = free variable\n", cVar + 1);
            printf("Please define test 2 var %d\n", cVar + 1);
            scanf("%f", &WeightsArray[cVar]);
            printf("--------------------\n");
        }
        cVar++;
        printf("\n");
    }

    cVar = 0;
    checkRow = 0;
    while (cVar < collumnAmount - 1)
    {
        if (checkRow > rowNumber - 1)
        {
            cVar++;
            continue;
        }
        int found = 1;

        if (equations[checkRow][cVar] > 0.001f)
        {
            found = 0;
            if (fabsf(equations[checkRow][collumnAmount - 1]) > 0.001f)
                WeightsArray[cVar] += equations[checkRow][collumnAmount - 1];

            else
                WeightsArray[cVar] = 0;

            for (int g = cVar + 1; g < collumnAmount - 1; g++)
                WeightsArray[cVar] += -WeightsArray[g] * equations[checkRow][g];

            checkRow++;
        }
        cVar++;
    }

    printf("(");
    for (int i = 0; i < collumnAmount - 1; i++)
    {
        printf("%f", WeightsArray[i]);
        if (i != collumnAmount - 2)
        {
            printf(", ");
        }
    }
    printf(")\n");
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
    int basicVaribles;
    int freeVariables;
    int success = 0;
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
            break;
        }

        PrintSystem(equations, amountOfRows, amountOfCollumns);
    }

    EchelonForm(equations, amountOfRows, amountOfCollumns, &success);
    if (success == 1)
        return 0;

    PrintWeights(equations, amountOfRows, amountOfCollumns, &basicVaribles, &freeVariables);
    float *WeightsArray = malloc(sizeof(float) * amountOfCollumns);
    if (freeVariables > 0 && basicVaribles > 0)
    {
        int input = -1;
        do
        {
            printf("Do you want to calculate a specific solution set?\n1.Yes\n2.No\n");
            scanf("%d", &input);
            if (input == 1)
            {
                CalcResult(equations, amountOfRows, amountOfCollumns, WeightsArray);
            }
            else if (input == 2)
            {
                break;
            }
            else
            {
                printf("Please pick valid option\n");
            }

        } while (1 == 1);
    }
    free(WeightsArray);
    return 0;
}
