class Register {
  private:
    unsigned char Buffer[ LEN_SIGNAL ];
  public:
    Register();
    ~Register();
    void add( unsigned char Data );
    void send( void );
};
