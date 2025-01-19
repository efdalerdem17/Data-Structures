#include <iostream>
#include <fstream> //file stream
#include <string>
#include "stack.h"
#include "stack.cpp"

/*
 * This code is prepared by Ayşe Efdal Erdem for the CS300 course at Sabancı University.
 * It solves the N-Queens problem using stack and backtracking algorithms with positive integers provided by the user.
 */

// boardstate shows the state of the chessboard at any point
struct Boardstate {
    int row;                            // current row
    int col;                            // current column
    std::vector<int> queensPositions;   // positions of queens on the board
    std::vector<std::vector<int>> safeMatrix;  // matrix tracking to place a queen on a safe position

    // constructor
    Boardstate(int n) : row(0), col(0),
                   queensPositions(n, -1),
                   safeMatrix(n, std::vector<int>(n, 0)) {}
};
// this class will solveNQueens the nqueens problem
class NQueensBacktracker {
private:
    int n;  // size of the chessboard
    Stack<Boardstate> stack;  // stack for backtracking algorithm
    std::vector<std::vector<int>> solutions;  // 2 D vector to store all found solutions
// Updates the safe matrix when the queen is placed or removed
    void updateSafeMatrix(std::vector<std::vector<int>>& matrix, int row, int col, int delta) {
        // Update rows and columns
        for (int i = 0; i < n; i++) {
            //+1 delta when placing a queen, -1 when removing a queen
            matrix[row][i] += delta;  // Update cells in the row
            matrix[i][col] += delta;  // Update cells in the column
        }

        // update main diagonal
        for (int i = 0; i < n; i++) {
            int j = col + (i - row);
            if (j >= 0 && j < n) {
                matrix[i][j] += delta;
            }
        }

        // update secondary diagonal
        for (int i = 0; i < n; i++) {
            int j = col - (i - row);
            if (j >= 0 && j < n) {
                matrix[i][j] += delta;
            }
        }

        matrix[row][col] -= 3 * delta;  // correct the queen's position
    }
// checks if a given position on the board is safe for placing a queen
    bool isPositionSafe(const std::vector<std::vector<int>>& matrix, int row, int col) {
        return matrix[row][col] == 0;
    }

public:
    //constructor
    NQueensBacktracker(int size) : n(size) {}
// solve with stack logic
    void solveNQueens() {
        Boardstate initialBoardBoardstate(n);
        stack.push(initialBoardBoardstate);

        while (!stack.empty()) {
            Boardstate currentBoardstate = stack.top(); //retrieve the current state from top
            stack.pop();  // remove

            // if all queens are placed --> solution
            if (currentBoardstate.row == n) {
                solutions.push_back(currentBoardstate.queensPositions);
                continue;
            }

            // placing a queen in each column of the current row
            for (; currentBoardstate.col < n; currentBoardstate.col++) {
                if (isPositionSafe(currentBoardstate.safeMatrix, currentBoardstate.row, currentBoardstate.col)) {
                    // create a new Boardstate for the next row
                    Boardstate newBoardstate = currentBoardstate;

                    // place the queen and update the safe matrix
                    newBoardstate.queensPositions[currentBoardstate.row] = currentBoardstate.col;
                    updateSafeMatrix(newBoardstate.safeMatrix, currentBoardstate.row, currentBoardstate.col, 1);

                    // move to next row
                    newBoardstate.row++;
                    // reset column counter
                    newBoardstate.col = 0;

                    // oush this new Boardstate onto the stack
                    stack.push(newBoardstate);
                }
            }
        }
    }

    void writeSolutionsToFile() const {
        std::string filename = std::to_string(n) + "queens_solutions.txt";
        std::ofstream outFile(filename);

        if (!outFile.is_open()) {
            std::cerr << "Error: Unable to open file for writing solutions.\n";
            return;
        }

        outFile << "Total solutions for N=" << n << ": " << solutions.size() << "\n\n";

        for (int i = solutions.size() - 1; i >= 0; i--) {
            outFile << "Solution " << (solutions.size() - i) << ": [";
            for (size_t j = 0; j < solutions[i].size(); j++) {
                outFile << solutions[i][j];
                if (j < solutions[i].size() - 1) {
                    outFile << ", ";
                }
            }
            outFile << "]\n";
        }
        outFile.close();
    }
};

int main() {
    int n;
    std::cout << "Enter the value of N for the N-Queens problem: ";
    std::cin >> n;

    if (n <= 0) {
        std::cout << "Invalid input. Please enter a positive integer\n";
        return 1;
    }

    if (n >= 14) {
        std::cout << "N should be less than 14\n";
        return 1;
    }

    NQueensBacktracker solveNQueensr(n);
    solveNQueensr.solveNQueens();
    solveNQueensr.writeSolutionsToFile();

    std::cout << "Solutions have been saved to '" << n << "queens_solutions.txt'\n";

    return 0;
}