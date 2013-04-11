#ifndef PROPUTILS_H
#define PROPUTILS_H

#define SIMPLE_NOTIFYING_PROPERTY(type, name) \
public: \
    Q_PROPERTY(type name READ name WRITE name##Setter NOTIFY name##Changed) \
    Q_SIGNAL void name##Changed(); \
    type name() { return m_##name; } \
    void name##Setter(type name) { if(m_##name != name) { m_##name = name; emit name##Changed(); } }\
protected: \
    type m_##name;

#define SIMPLE_PROPERTY(type, name) \
public: \
    Q_PROPERTY(type name READ name WRITE name##Setter CONSTANT) \
    type name() { return m_##name; } \
    void name##Setter(type name) {m_##name = name; }\
protected: \
    type m_##name;


#define SIMPLE_PROPERTY_WDECL(type, name) \
public: \
    Q_PROPERTY(type name READ name WRITE name##Setter CONSTANT) \
    type name() { return m_##name; } \
    void name##Setter(type name);\
protected: \
    type m_##name;


#define SIMPLE_NOTIFYING_PROPERTY_DECL(type, name) \
public: \
    Q_PROPERTY(type name READ name WRITE name##Setter NOTIFY name##Changed) \
    Q_SIGNAL void name##Changed(); \
    type name(); \
    void name##Setter(type name);

#define SIMPLE_PROPERTY_DECL(type, name) \
public: \
    Q_PROPERTY(type name READ name WRITE name##Setter CONSTANT) \
    type name(); \
    void name##Setter(type name);


#endif // PROPUTILS_H
