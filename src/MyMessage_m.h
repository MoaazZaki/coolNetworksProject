//
// Generated file, do not edit! Created by nedtool 5.6 from MyMessage.msg.
//

#ifndef __MYMESSAGE_M_H
#define __MYMESSAGE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include <bitset>
typedef  std::bitset<8> bits;
typedef  int event;
// }}

/**
 * Class generated from <tt>MyMessage.msg:9</tt> by nedtool.
 * <pre>
 * packet MyMessage
 * {
 *     int Seq_Num;
 *     int Received_Frames_Count;
 *     int Char_Count;
 *     string M_Payload;
 *     bits mycheckbits;
 *     event E_Type;
 *     int ack;
 * 
 * }
 * </pre>
 */
enum eventType {NETWORK_LAYER_READY,FRAME_ARRIVAL,ERR,TIMEOUT,CONTROL,FINISH};

class MyMessage : public ::omnetpp::cPacket
{
  protected:
    int Seq_Num;
    int Received_Frames_Count;
    int Char_Count;
    ::omnetpp::opp_string M_Payload;
    bits mycheckbits;
    event E_Type;
    int ack;

  private:
    void copy(const MyMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const MyMessage&);

  public:
    MyMessage(const char *name=nullptr, short kind=0);
    MyMessage(const MyMessage& other);
    virtual ~MyMessage();
    MyMessage& operator=(const MyMessage& other);
    virtual MyMessage *dup() const override {return new MyMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getSeq_Num() const;
    virtual void setSeq_Num(int Seq_Num);
    virtual int getReceived_Frames_Count() const;
    virtual void setReceived_Frames_Count(int Received_Frames_Count);
    virtual int getChar_Count() const;
    virtual void setChar_Count(int Char_Count);
    virtual const char * getM_Payload() const;
    virtual void setM_Payload(const char * M_Payload);
    virtual bits& getMycheckbits();
    virtual const bits& getMycheckbits() const {return const_cast<MyMessage*>(this)->getMycheckbits();}
    virtual void setMycheckbits(const bits& mycheckbits);
    virtual event& getE_Type();
    virtual const event& getE_Type() const {return const_cast<MyMessage*>(this)->getE_Type();}
    virtual void setE_Type(const event& E_Type);
    virtual int getAck() const;
    virtual void setAck(int ack);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const MyMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, MyMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef __MYMESSAGE_M_H

