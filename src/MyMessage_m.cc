//
// Generated file, do not edit! Created by nedtool 5.6 from MyMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "MyMessage_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(MyMessage)

MyMessage::MyMessage(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->Seq_Num = 0;
    this->Received_Frames_Count = 0;
    this->Char_Count = 0;
    this->ack = 0;
    this->M_type = 0;
}

MyMessage::MyMessage(const MyMessage& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

MyMessage::~MyMessage()
{
}

MyMessage& MyMessage::operator=(const MyMessage& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void MyMessage::copy(const MyMessage& other)
{
    this->Seq_Num = other.Seq_Num;
    this->Received_Frames_Count = other.Received_Frames_Count;
    this->Char_Count = other.Char_Count;
    this->M_Payload = other.M_Payload;
    this->mycheckbits = other.mycheckbits;
    this->E_Type = other.E_Type;
    this->ack = other.ack;
    this->M_type = other.M_type;
}

void MyMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->Seq_Num);
    doParsimPacking(b,this->Received_Frames_Count);
    doParsimPacking(b,this->Char_Count);
    doParsimPacking(b,this->M_Payload);
    doParsimPacking(b,this->mycheckbits);
    doParsimPacking(b,this->E_Type);
    doParsimPacking(b,this->ack);
}

void MyMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->Seq_Num);
    doParsimUnpacking(b,this->Received_Frames_Count);
    doParsimUnpacking(b,this->Char_Count);
    doParsimUnpacking(b,this->M_Payload);
    doParsimUnpacking(b,this->mycheckbits);
    doParsimUnpacking(b,this->E_Type);
    doParsimUnpacking(b,this->ack);
}

int MyMessage::getSeq_Num() const
{
    return this->Seq_Num;
}

void MyMessage::setSeq_Num(int Seq_Num)
{
    this->Seq_Num = Seq_Num;
}

int MyMessage::getReceived_Frames_Count() const
{
    return this->Received_Frames_Count;
}

void MyMessage::setReceived_Frames_Count(int Received_Frames_Count)
{
    this->Received_Frames_Count = Received_Frames_Count;
}

int MyMessage::getChar_Count() const
{
    return this->Char_Count;
}

void MyMessage::setChar_Count(int Char_Count)
{
    this->Char_Count = Char_Count;
}

const char * MyMessage::getM_Payload() const
{
    return this->M_Payload.c_str();
}

void MyMessage::setM_Payload(const char * M_Payload)
{
    this->M_Payload = M_Payload;
}

bits& MyMessage::getMycheckbits()
{
    return this->mycheckbits;
}

void MyMessage::setMycheckbits(const bits& mycheckbits)
{
    this->mycheckbits = mycheckbits;
}

event& MyMessage::getE_Type()
{
    return this->E_Type;
}

void MyMessage::setE_Type(const event& E_Type)
{
    this->E_Type = E_Type;
}

int MyMessage::getAck() const
{
    return this->ack;
}

void MyMessage::setAck(int ack)
{
    this->ack = ack;
}

int MyMessage::getM_type() const
{
    return this->M_type ;
}

void MyMessage::setM_type(int type)
{
    this->M_type = type;
}

class MyMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    MyMessageDescriptor();
    virtual ~MyMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(MyMessageDescriptor)

MyMessageDescriptor::MyMessageDescriptor() : omnetpp::cClassDescriptor("MyMessage", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

MyMessageDescriptor::~MyMessageDescriptor()
{
    delete[] propertynames;
}

bool MyMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<MyMessage *>(obj)!=nullptr;
}

const char **MyMessageDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *MyMessageDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int MyMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount() : 7;
}

unsigned int MyMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *MyMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "Seq_Num",
        "Received_Frames_Count",
        "Char_Count",
        "M_Payload",
        "mycheckbits",
        "E_Type",
        "ack",
    };
    return (field>=0 && field<7) ? fieldNames[field] : nullptr;
}

int MyMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='S' && strcmp(fieldName, "Seq_Num")==0) return base+0;
    if (fieldName[0]=='R' && strcmp(fieldName, "Received_Frames_Count")==0) return base+1;
    if (fieldName[0]=='C' && strcmp(fieldName, "Char_Count")==0) return base+2;
    if (fieldName[0]=='M' && strcmp(fieldName, "M_Payload")==0) return base+3;
    if (fieldName[0]=='m' && strcmp(fieldName, "mycheckbits")==0) return base+4;
    if (fieldName[0]=='E' && strcmp(fieldName, "E_Type")==0) return base+5;
    if (fieldName[0]=='a' && strcmp(fieldName, "ack")==0) return base+6;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *MyMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "string",
        "bits",
        "event",
        "int",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : nullptr;
}

const char **MyMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *MyMessageDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int MyMessageDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    MyMessage *pp = (MyMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *MyMessageDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    MyMessage *pp = (MyMessage *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string MyMessageDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    MyMessage *pp = (MyMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSeq_Num());
        case 1: return long2string(pp->getReceived_Frames_Count());
        case 2: return long2string(pp->getChar_Count());
        case 3: return oppstring2string(pp->getM_Payload());
        case 4: return pp->getMycheckbits().to_string();
        case 5: return long2string(pp->getE_Type());
        case 6: return long2string(pp->getAck());
        default: return "";
    }
}

bool MyMessageDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    MyMessage *pp = (MyMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setSeq_Num(string2long(value)); return true;
        case 1: pp->setReceived_Frames_Count(string2long(value)); return true;
        case 2: pp->setChar_Count(string2long(value)); return true;
        case 3: pp->setM_Payload((value)); return true;
        case 6: pp->setAck(string2long(value)); return true;
        default: return false;
    }
}

const char *MyMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 4: return omnetpp::opp_typename(typeid(bits));
        case 5: return omnetpp::opp_typename(typeid(event));
        default: return nullptr;
    };
}

void *MyMessageDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    MyMessage *pp = (MyMessage *)object; (void)pp;
    switch (field) {
        case 4: return (void *)(&pp->getMycheckbits()); break;
        case 5: return (void *)(&pp->getE_Type()); break;
        default: return nullptr;
    }
}


