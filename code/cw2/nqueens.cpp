#include <omp.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;

bool boardIsValidSoFar(int lastPlacedRow, const std::vector<int>& gameBoard)
{
    const auto N = gameBoard.size();
    int lastPlacedColumn = gameBoard[lastPlacedRow];

    for (int row = 0; row < lastPlacedRow; ++row)
    {
        if (gameBoard[row] == lastPlacedColumn)
            return false;
        const auto col1 = lastPlacedColumn - (lastPlacedRow - row);
        const auto col2 = lastPlacedColumn + (lastPlacedRow - row);
        if (gameBoard[row] == col1 || gameBoard[row] == col2)
            return false;
    }
    return true;
}

void calculateSolutionsRecursive(int writeToRow, std::vector<int>& gameBoard, int N, std::vector<std::vector<int>>& solutions)
{
    // Parallelize the inner loop
    #pragma omp parallel for
    for (int i = 0; i < N; ++i)
    {
        gameBoard[writeToRow] = i;
        if (boardIsValidSoFar(writeToRow, gameBoard))
        {
            const auto nextWriteToRow = writeToRow + 1;
            if (nextWriteToRow != N)
                calculateSolutionsRecursive(nextWriteToRow, gameBoard, N, solutions);
            else
            {
                // Critical section to avoid race condition when updating solutions vector
                #pragma omp critical
                solutions.push_back(gameBoard);
            }
        }
    }
}

void calculateAllSolutions(int N, bool print)
{
    std::vector<std::vector<int>> solutions;
    std::vector<int> gameBoard(N, 0);

    // Parallelize the outer loop
    #pragma omp parallel for
    for (int i = 0; i < N; ++i)
    {
        calculateSolutionsRecursive(0, gameBoard, N, solutions);
    }

    if (print)
    {
        std::string text;
        text.resize(N * (N + 1) + 1);
        text.back() = '\n';
        for (const auto& solution : solutions)
        {
            for (int i = 0; i < N; ++i)
            {
                auto queenAtRow = solution[i];
                for (int j = 0; j < N; ++j)
                    text[i * (N + 1) + j] = queenAtRow == j ? 'X' : '.';
                text[i * (N + 1) + N] = '\n';
            }
            std::cout << text << "\n";
        }
    }
}

int main(int argc, char** argv)
{
    // Parallelize the main loop
    #pragma omp parallel for
    for (int N = 4; N < 13; ++N)
        calculateAllSolutions(N, false);
}
