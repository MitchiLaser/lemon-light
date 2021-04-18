class Timer {
  private:
    unsigned long lastUpdate, delay;
  public:
    Timer(unsigned long Delay);
    ~Timer();
    void reset();
    bool expired();
};
