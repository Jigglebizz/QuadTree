#include <stdio.h>
#include <iostream>
#include <tuple>

template <typename T>
class QuadTree {
private:
    struct data_elem_t {
        data_elem_t( const T& Data, unsigned X, unsigned Y) 
          : data(Data),
            x(X), y(Y)
        { }
        
        const T& data;
        unsigned x, y;
    };

    class Node {
    private:
        QuadTree::Node *tr, *tl, *br, *bl;
        data_elem_t* mData;
        unsigned mX, mY, mW, mH;
        
        std::tuple<QuadTree::Node**, unsigned, unsigned, unsigned, unsigned> getQuadrant(unsigned X, unsigned Y) {
            unsigned halfW = mW / 2;
            unsigned halfH = mH / 2;
            
            if (X <= mX + halfW) {
                if (Y < mY + halfH) {
                    return std::make_tuple(&tl, mX, mY, halfW, halfH);
                }
                else {
                    return std::make_tuple(&bl, mX, mY + halfH, halfW, halfH);
                }
            }
            else {
                if (Y < mY + halfH) {
                    return std::make_tuple(&tr, mX + halfW, mY, halfW, halfH);
                }
                else {
                    return std::make_tuple(&br, mX + halfW, mY + halfH, halfW, halfH);
                }
            }
        }
        
        std::string quadInfo() const {
            return std::to_string(mX) + "," + std::to_string(mY) + " : " + std::to_string(mW) + "," + std::to_string(mH);
        }
    public:
        Node( data_elem_t* Data, unsigned X, unsigned Y, unsigned W, unsigned H)
          : mData(Data),
            mX(X), mY(Y), mW(W), mH(H),
            tr(nullptr),
            tl(nullptr),
            br(nullptr),
            bl(nullptr)
        { 
          // std::cout << "New Node Created: " << Data->data << ": " << Data->x << "," << Data->y << std::endl;
        }
        ~Node() {
            if (mData != nullptr) {
                delete mData;
            }
            if (tr != nullptr) {
                delete tr;
            }
            if (tl != nullptr) {
                delete tl;
            }
            if (br != nullptr) {
                delete br;
            }
            if (bl != nullptr) {
                delete bl;
            }
        }
        
        void insert(const T& Item, unsigned X, unsigned Y) {
            // We are splitting this node
            if (mData != nullptr) {
                Node** newQuadA;
                Node** newQuadB;
                unsigned xa, ya, wa, ha,
                         xb, yb, wb, hb;
                
                std::tie(newQuadA, xa, ya, wa, ha) = getQuadrant(mData->x, mData->y);
                std::tie(newQuadB, xb, yb, wb, hb) = getQuadrant(X, Y);
                
                if (newQuadA == newQuadB) {
                    *newQuadA = new Node(mData, xa, ya, wa, ha);
                    (*newQuadA)->insert(Item, X, Y);
                } else {
                    *newQuadA = new Node(mData, xa, ya, wa, ha);
                    *newQuadB = new Node(new data_elem_t(Item, X, Y), xb, yb, wb, hb);
                }
                
                mData = nullptr;
            } else { // or passing it down
                Node** newQuad;
                unsigned x, y, w, h;
                
                std::tie(newQuad, x, y, w, h) = getQuadrant(X, Y);
                if (newQuad == nullptr) {
                    *newQuad = new Node(new data_elem_t(Item, X, Y), x, y, w, h);
                } else {
                    (*newQuad)->insert(Item, X, Y);
                }
            }
        }
        
        void print(unsigned indentation = 0) {
            std::cout << std::string(indentation, ' ') << "Quadrant: " << quadInfo() << std::endl;
            if (mData != nullptr) {
                std::cout << std::string(indentation, ' ') << mData->data << ": " << mData->x << ", " << mData->y << " at " << mX << "," << mY << " : " << mW << "," << mH << std::endl;
            }
            else {
                std::cout << std::string(indentation, ' ') << "tl: " << ((tl == nullptr) ? "empty" : "") << std::endl;
                if (tl != nullptr) {
                    tl->print(indentation + 1);
                }
                
                std::cout << std::string(indentation, ' ') << "tr: " << ((tr == nullptr) ? "empty" : "") << std::endl;
                if (tr != nullptr) {
                    tr->print(indentation + 1);
                }
                
                std::cout << std::string(indentation, ' ') << "bl: " << ((bl == nullptr) ? "empty" : "") << std::endl;
                if (bl != nullptr) {
                    bl->print(indentation + 1);
                }
                
                std::cout << std::string(indentation, ' ') << "br: " << ((br == nullptr) ? "empty" : "") << std::endl;
                if (br != nullptr) {
                    br->print(indentation + 1);
                }
            }
        }
    };
    
    Node* mRoot = nullptr;
    unsigned mWidth, mHeight;
public:
    QuadTree(unsigned Width, unsigned Height)
      : mWidth(Width), mHeight(Height)
     {
     }
    ~QuadTree() {
        delete mRoot;
    }
    void insert(const T& Item, unsigned X, unsigned Y) {
        if (mRoot == nullptr) {
            mRoot = new Node(new data_elem_t(Item, X, Y), 0, 0, mWidth, mHeight);
        }
        else {
            mRoot->insert(Item, X, Y);
        }
    }
    
    void print() {
        std::cout << "Printing Quad Tree" << std::endl;
        if (mRoot != nullptr) {
            mRoot->print();
        }
        else {
            std::cout << "Empty!" << std::endl;
        }
    }
};

int main()
{
    QuadTree<int> qt(100, 100);
    qt.insert(0, 10, 10);
    qt.insert(0, 90, 90);
    qt.insert(1, 80, 80);
    qt.print();

    return 0;
}
