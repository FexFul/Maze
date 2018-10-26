#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

enum class MazeNodeType {
    WALL, POSSIBLE_PATH, USED_PATH, START, FINISH
};

class MazeNode {
public:
    MazeNode(MazeNodeType type, int x, int y) : type(type), x(x), y(y) {}

    MazeNode() {}

    MazeNodeType getType() const {
        return type;
    }

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    void setType(MazeNodeType type) {
        MazeNode::type = type;
    }

private:
    MazeNodeType type;
    int x, y;
};

class Maze {
private:
    vector<vector<MazeNode>> entities;
    MazeNode start, finish;

    vector<pair<int, int>> solution;

public:
    void fromFile(const string &filename) {
        ifstream input(filename);
        string line;

        int y = 0;
        while (input >> line) {
            vector<MazeNode> entityRow;

            int x = 0;
            for (auto symbol : line) {
                switch (symbol) {
                    case '_':
                        entityRow.emplace_back(MazeNodeType::POSSIBLE_PATH, x, y);
                        break;
                    case '#':
                        entityRow.emplace_back(MazeNodeType::WALL, x, y);
                        break;
                    case 'S':
                        entityRow.emplace_back(MazeNodeType::START, x, y);
                        start = entityRow.back();
                        break;
                    case 'E':
                        entityRow.emplace_back(MazeNodeType::FINISH, x, y);
                        finish = entityRow.back();
                        break;
                }

                x += 1;
            }

            this->entities.push_back(entityRow);
            y += 1;
        }
    }

    bool findPath(int x, int y) {

        // if (x,y outside maze) return false
        //if (x,y is goal) return true
        //if (x,y not open) return false
        //mark x,y as part of solution path
        //if (FIND-PATH(North of x,y) == true) return true
        //if (FIND-PATH(East of x,y) == true) return true
        //if (FIND-PATH(South of x,y) == true) return true
        //if (FIND-PATH(West of x,y) == true) return true
        //unmark x,y as part of solution path
        //return false

        if (x < 0 || x > getWidth() || y < 0 || y > getHeight()) {
            return false;
        }

        if (x == this->finish.getX() && y == this->finish.getY()) {
            return true;
        }

        if ((entities.at(y).at(x).getType() == MazeNodeType::WALL) ||
            (entities.at(y).at(x).getType() == MazeNodeType::USED_PATH)) {
            return false;
        }

        entities.at(y).at(x).setType(MazeNodeType::USED_PATH);

        // north
        if (findPath(x, y - 1)) {
            return true;
        }
        // east
        if (findPath(x + 1, y)) {
            return true;
        }

        // south
        if (findPath(x, y + 1)) {
            return true;
        }
        // west
        if (findPath(x - 1, y)) {
            return true;
        }

        // unmark
        entities.at(y).at(x).setType(MazeNodeType::POSSIBLE_PATH);

        return false;
    }

    void findPath() {
        findPath(this->start.getX(), this->start.getY());
    }

    void dump() {
        for (auto row : this->entities) {
            for (auto entity : row) {
                switch (entity.getType()) {
                    case MazeNodeType::WALL:
                        cout << "#";
                        break;
                    case MazeNodeType::POSSIBLE_PATH:
                        cout << "_";
                        break;
                    case MazeNodeType::USED_PATH:
                        cout << "*";
                        break;
                    case MazeNodeType::START:
                        cout << "S";
                        break;
                    case MazeNodeType::FINISH:
                        cout << "E";
                        break;
                }
            }
            cout << endl;
        }
    }

    unsigned long getHeight() {
        return entities.size();
    }

    unsigned long getWidth() {
        return entities.at(entities.size() - 1).size();
    }
};

using namespace std;

int main() {
    Maze maze;
    maze.fromFile("maze");
    maze.findPath();
    maze.dump();
    return 0;
}