class BNode {
  private:
    unsigned char Data;
    BNode * Next;
  public:
    BNode( unsigned char d );
    ~BNode( void );
    unsigned char getData( void );
    void setData( unsigned char d);
    BNode *getNext( void );
    void setNext( BNode *New );
};

class Buffer {
  private:
    BNode *front, *back;
  public:
    Buffer();
    ~Buffer();
    void push( unsigned char New );
    unsigned char pop( void );
    bool empty( void );
};
