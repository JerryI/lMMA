class Logger {
public:
    Logger(ostream& os) : os_(os), curIndentLevel_(0) {}
    void increaseLevel() { ++curIndentLevel_; }
    void decreaseLevel() { --curIndentLevel_; }

private:
    template<typename T> friend ostream& operator<<(Logger&, T);

    ostream& os_;
    int curIndentLevel_;
};

template<typename T> 
ostream& operator<<(Logger& log, T op) {
    for(int i = 0; i < log.curIndentLevel_ * 4; ++i) {
        log.os_ << ' ';
    }
    log.os_ << op;
    return log.os_;
}