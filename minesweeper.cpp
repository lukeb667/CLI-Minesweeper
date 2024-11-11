// Luke Becker
// Simple console-based minesweeper program. 
//

#include <vector>
#include <tuple>
#include <iostream>
#include <time.h> 

using namespace std;

class GameGrid {
    public:
        int sizeX; // Width of the game grid
        int sizeY; // Height of the game grid
        int numMines; // Literal number of mines to place on the grid
        double mineDensity; // Percentage of the grid to fill with mines, 0 for none, 1 for full 

        vector<tuple<int, int>> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}, {-1,-1}, {-1,1}, {1,-1}, {1,1}}; // Each direction to check when looking at bordering cells
        vector<vector<tuple<int, int>>> grid; // Grid is represented as a nested vector of xy coordinates

    public: bool setMine(int x, int y) {
        if (get<0>(grid[x][y]) == 9) { // Nine represents a prexisting mine. Return false becase we can't place here. 
            return false; }
        else { // Otherwise we set the tile to a mine. 
            grid[x][y] = {9, get<1>(grid[x][y])}; }
            for (unsigned int i = 0; i < directions.size(); i++) { 
                // And increment the value of each surrounding. Check for border cases here. 
                if ((0 <= x + get<0>(directions[i]) && x + get<0>(directions[i]) < sizeX) && (0 <= y + get<1>(directions[i]) && y + get<1>(directions[i]) < sizeY) && get<0>(grid[x + get<0>(directions[i])][y + get<1>(directions[i])]) != 9) {
                    get<0>(grid[x + get<0>(directions[i])][y + get<1>(directions[i])]) += 1; }
            }
        return true; }
    
    public: int getInput(string prompt, int lowerb, int upperb) {
        // Get user input for which cell to open
        string inputVal;
        cout << prompt;
        cin >> inputVal;
        int intInputVal = stoi(inputVal);

        // If the input is outside the bounds, re-prompt
        if (intInputVal > upperb || intInputVal < lowerb) {
            cout << "Error: bad input domain\n";
            return getInput(prompt, lowerb, upperb); }
        return int(intInputVal) - 1; // -1 because the cell labels start at 1 for end-user happiness
    }
    public: void openCell() {
        int x = -1;
        int y = -1;

        x = getInput("X: ", 1, sizeX); // Get the x-coord of the cell to open 
        y = getInput("Y: ", 1, sizeY); // Get the y-coord of the cell to open 
        
        // Recursively open the surrounding cells if they contain a zero. Bombs are handled elsewhere. 
        if (get<1>(grid[x][y]) == 1) {
            grid[x][y] = {get<0>(grid[x][y]), 0};
            if (get<0>(grid[x][y]) == 0) {                 
                for (unsigned int i = 0; i < directions.size(); i++) {
                    if ((0 <= x + get<0>(directions[i]) && x + get<0>(directions[i]) < sizeX) && (0 <= y + get<1>(directions[i]))) {
                        openCell(x + get<0>(directions[i]), y + get<1>(directions[i])); }
                } } }
        }
    
    public: void openCell(int x, int y) {
        // Open a cell given the x and y coords. Called by the recursive cell opening. 
        if (get<1>(grid[x][y]) == 1) {
            grid[x][y] = {get<0>(grid[x][y]), 0}; 
            if (get<0>(grid[x][y]) == 0) {
                for (unsigned int i = 0; i < directions.size(); i++) {
                    if ((0 <= x + get<0>(directions[i]) && x + get<0>(directions[i]) < sizeX) && (0 <= y + get<1>(directions[i]))) {
                        openCell(x + get<0>(directions[i]), y + get<1>(directions[i])); }
                } } }
            
        }


    public: bool displayGrid() {
        bool exploded = false;
        bool unopenedCells = false;

        // x Labels 
        cout << "    ";
        for (int x = 1; x <= sizeX; x++) {
            if      (x < 10)   {cout << x << "   ";} 
            else if (x < 100)  {cout << x << "  ";}
            else if (x < 1000) {cout << x << " ";} }
        cout << "x \n    ";

        // Decoration
        for (int x = 1; x <= sizeX; x++) {cout << "_" << "   ";}
        cout << "\n";

        for (int y = 0; y < sizeY; y++) { // Rows 
            // Decoration
            if      (y+1 < 10)   {cout << y+1 << "  |";}
            else if (y+1 < 100)  {cout << y+1 << " |";}
            else if (y+1 < 1000) {cout << y+1 << "|";} 

            // Print the grid
            for (int x = 0; x < sizeX; x++) { // Columns
                if (get<1>(grid[x][y]) == 0) {
                    if (get<0>(grid[x][y]) == 9) {cout << "B" << "   "; exploded = true;} // If one of the revealed tiles contains a bomb, you lose!
                    else {cout << get<0>(grid[x][y]) << "   "; } }
                else if (get<1>(grid[x][y]) == 1) {
                    cout << "#" << "   "; 
                    if (get<0>(grid[x][y]) != 9) {unopenedCells = true;} } // If there are unopened cells, the game must go on!
                else if (get<1>(grid[x][y]) == 2) {
                    cout << "?" << "   "; 
                    if (get<0>(grid[x][y]) != 9) {unopenedCells = true;} } // If there are unopened cells, the game must go on!
                else if (get<1>(grid[x][y]) == 3) {
                    cout << "!" << "   ";
                    if (get<0>(grid[x][y]) != 9) {unopenedCells = true;} } } // If there are unopened cells, the game must go on!

            cout << "\n";
        }
        cout << "y\n";

        if (exploded == true) {cout << "You lose!"; return false;}
        else {
            if (unopenedCells == true) {return true;}
            else {cout << "You win!"; return false;}
        }
    }
    public: GameGrid() { 
        // Instantiate the grid with defaults
        srand(time(0)); 
        sizeX = 20;
        sizeY = 20;
        mineDensity = .1;
        numMines = (sizeX * sizeY) * mineDensity;

        for (int i = 0; i < sizeX; i++) {
            vector<tuple<int, int>> row;
            for (int j = 0; j < sizeY; j++) {
                row.push_back({0, 1}); }
            grid.push_back(row); }

        for (int i = 0; i < numMines; i++) {
            if (setMine(rand()%sizeX, rand()%sizeY) == false) {i--;}
        }
    }
    public: GameGrid(int setSizeX, int setSizeY, double setMineDensity) {
        // Instantiate the grid with arguments 
        srand(time(0)); 
        if (setSizeX > 200) {setSizeX = 200;} // Cap at 200
        if (setSizeY > 200) {setSizeY = 200;} // Cap at 200

        if (setSizeX < 1) {setSizeX = 1;} // Minimum 1 cell 
        if (setSizeY < 1) {setSizeX = 1;} // Minimum 1 cell

        sizeX = setSizeX;
        sizeY = setSizeY;

        mineDensity = setMineDensity;
        numMines = (sizeX * sizeY) * mineDensity;
        if (numMines == 0) {numMines = 1;} // Minimum 1 mine
        if (numMines > sizeX * sizeY) {numMines = sizeX * sizeY;}
        

        for (int i = 0; i < sizeX; i++) {
            vector<tuple<int, int>> row;
            for (int j = 0; j < sizeY; j++) {
                row.push_back({0, 1}); }
            grid.push_back(row); }

        for (int i = 0; i < numMines; i++) {
            if (setMine(rand()%sizeX, rand()%sizeY) == false) {i--;}
        }
    }
};

int main() {
    string setGridX;
    string setGridY;
    string setDensity;

    int gridX;
    int gridY;
    double density;

    // Initalize with user specifications
    cout << "Set X (1-200):";
    cin >> setGridX;
    gridX = stoi(setGridX);

    cout << "Set Y (1-200):";
    cin >> setGridY;
    gridY = stoi(setGridY);

    cout << "Set Mine Density (0-1):";
    cin >> setDensity;
    density = stod(setDensity);
    cout << "\n\n";

    GameGrid gridOBJ(gridX, gridY, density);

    // Gameloop
    while (gridOBJ.displayGrid() == true) {
        gridOBJ.openCell(); }

    cout << "\nPress any key to exit..." << endl;
    
    string exitBuffer;
    cin >> exitBuffer;
    

    return 0;
}