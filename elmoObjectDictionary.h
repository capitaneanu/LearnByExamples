#pragma once

/*
    01/06/2021 VeysiAdn 
    Gold Solo Drive series , EtherCAT specific object dictionary based on;  
    ELMO EtherCAT Application Manual document. Starting from pg.69
    RxPDO refers commands from master to the slave 
    TxPDO refers responses from slave to the master 
    Client / Server communication model is defined.
    Server is slave and client is master.
*/


#define RxPDO_MAPPING
#ifdef  RxPDO_MAPPING

const unsigned int       defaultSubIndex = 0x00;

#define RX1605h            // Here you can define one index from Rx1600h - Rx1606h to exclude others.

// Note that from Rx1600h to Rx1606h, you can use just one of these RxPDO mapping others will be excluded. \
e.g. if you use Rx1605h, you can't use 1600h to 1604h and 1606h. 
// and you can't map more than 32 bytes.
/*
The total bytes which can be mapped by the Gold line EtherCAT is 32 bytes.
Objects between 0x1A00 (TxPDO1) to 0x1A09 are excluded from each other. Only one can
be selected at a time.
Objects between 0x1600 (RxPDO1) to 0x1606 are excluded from each other, only one can be selected at a time. Ref* Elmo EtherCAT Applcation Manual document; pg.69
The drive performs a single process in a cycle. In a situation where the same object is
mapped twice, the last received object will be processed.
*/
#ifdef RX1600h
class Rx1600h  
{
    public:
    const int            targetPosition  = 0x607A ;
    const int            digitalOutputs  = 0x60FE ;
    const unsigned short controlWord     = 0x6040 ; 

};
#endif

#ifdef RX1601h

class  Rx1601h 
{
    public:
    const int            targetVelocity  = 0x6FF  ;
    const unsigned short controlWord     = 0x6040 ; 
};
#endif

#ifdef RX1602h
class Rx1602h
{
    public:
    const int            targetTorque    = 0x6071 ;
    const unsigned short controlWord     = 0x6040 ; 
};
#endif

#ifdef RX1603h
class Rx1603h
{
    public:
    const int            targetPosition    = 0x6071 ;
    const unsigned int   digitalOutputs    = 0x60FE ;
    const int            velocityOffset    = 0x60B1 ;
    const unsigned short controlWord       = 0x6040 ; 
};
#endif

#ifdef RX1604h
class Rx1604h
{
    public:
    const int            targetPosition  = 0x607A ;
    const int            targetVelocity  = 0x60FF ;
    const int            MaxTorque       = 0x6072 ;
    const unsigned short controlWord     = 0x6040 ; 
};
#endif

#ifdef RX1605h
class Rx1605h
{
    public:
    const int            targetPosition      = 0x607A ;
    const int            targetVelocity      = 0x60FF ;
    const int            targetTorque        = 0x6071 ;
    const int            MaxTorque           = 0x6072 ;
    const unsigned short controlWord         = 0x6040 ; 
    const unsigned short modesOfOperation    = 0x6060 ;    // 8 bit
};
#endif

#ifdef RX1606h
class Rx1606h
{
    public:
    const int            targetPosition      = 0x607A ;
    const int            targetVelocity      = 0x60FF ;
    const unsigned int   digitalOutputs      = 0x60FE ;
    const int            velocityOffset      = 0x60B1 ;
    const int            torqueOffset        = 0X60B2 ;
    const unsigned short controlWord         = 0x6040 ;
};
#endif

class Rx160Ah
{
    public:
    const unsigned short controlWord = 0x6040; 
};

class Rx160Bh
{
    public:
    const unsigned short modesOfOperation = 0X6060; 
};

class Rx160Ch
{
    public:
    const short targetTorque       = 0x6071; 
};

class Rx160Dh
{
    public:
    const short maxTorque          = 0x6072; 
};

class Rx160Eh
{
    public: 
    const short  maxCurrent        = 0X6073; 
};

class Rx160Fh
{
    public:
    const int targetPosition = 0x607A; 
};

class Rx1610h
{
    public:
    const int maxProfileVelocity = 0x607F; 
};

class Rx1611h
{
    public:
    const int profileVelocity      = 0x6040; 
};

class Rx1612h
{
    public:
    const int endVelocity         = 0x6082; 
};

class Rx1613h
{
    public:
    const int profileAcceleration = 0x6083; 
};

class Rx1614h
{
    public:
    const int profileDeceleration = 0x6084; 
};

class Rx1615h
{
    public:
    const int torqueSlope         = 0x6087; 
};

class Rx1616h
{
    public:
    const int positionOffset         = 0x60B0; 
};

class Rx1617h
{
    public:
    const int velocityOffset      = 0x60B1; 
};

class Rx1618h
{
    public:
    const short torqueOffset             = 0x60B2; 
};

class Rx1619h
{
    public:
    const short touchProbeFunction        = 0x60B8; 
};

class Rx161Ah
{
    public:
    const int  interpolatedDataRecord_2 = 0x60C1; // 0x02 Subindex; 
};

class Rx161Bh
{
    public:
    const int interpolatedDatRecord_1 = 0x60C1;   // 0x01 Subindex;
};

class Rx161Ch
{
    public:
    const int           targetVelocity = 0x60FF; 
};

class Rx161Dh
{
    public:
    const unsigned int digitalOutputs = 0x60FE; 
};

class Rx161Eh
{
    public:
    const unsigned short polarity  = 0X607E;     // 8 bit
};
#endif


#define TxPDO_MAPPING
#ifdef TxPDO_MAPPING

/*
The total bytes which can be mapped by the Gold line EtherCAT is 32 bytes.
Objects between 0x1A00 (TxPDO1) to 0x1A09 are excluded from each other. Only one can
be selected at a time. Ref* Elmo EtherCAT Applcation Manual document; pg.69
The drive performs a single process in a cycle. In a situation where the same object is
mapped twice, the last received object will be processed.
*/

#define TX1A00h                  // You can define one PDO index to exclude others. e.g \
                                   here we defined 0x1A00h index to exclude 0x1A01 - 0x1A09 

#ifdef  TX1A00h
class  Tx1A00h
{
    public:
    const int            positionActualValue  = 0x6064 ;
    const int            digitalInputs        = 0x60FD ;
    const unsigned short statusWord          = 0x6041 ; 

};
#endif

#ifdef  TX1A01h
class Tx1A01h
{
    public:
    const int            positionActualValue  = 0x6064 ;
    const int            velocityDemandValue  = 0x606B ;
    const short          torqueDemandValue    = 0x6074 ;
    const unsigned short statusWord          = 0x6041 ; 

};
#endif

#ifdef  TX1A02h
class Tx1A02h
{
    public:
    const int            positionActualValue        = 0x6064 ;
    const short          torqueActualValue          = 0x6077 ;
    const short          modesOfOperationDisplay    = 0x6061 ;
    const unsigned short statusWord                = 0x6041 ; 

};
#endif

#ifdef  TX1A03h
class Tx1A03h
{
    public:
    const int            positionActualValue        = 0x6064 ;
    const int            digitalInputs              = 0x60FD ;
    const int            velocityActualValue        = 0x606C ;
    const unsigned short statusWord                = 0x6041 ; 

};
#endif

#ifdef  TX1A04h
class Tx1A04h
{
    public:
    const int            positionActualValue          = 0x6064 ;
    const int            positionFollowingErrorValue  = 0x60FD ;
    const short          torqueActualValue            = 0x6077 ;
    const unsigned short statusWord                  = 0x6041 ;
    const short          modesOfOperationDisplay      = 0x6061 ; // 8 bit

};
#endif


class Tx1A0Ah
{
    public:
    const short            statusWord              = 0x6041 ;
};

class Tx1A0Bh
{
    public:
    const short           modesOfOperationDisplay   = 0x6061 ;
};

class Tx1A0Ch
{
    public:
    const int             positionDemand            = 0x6062 ;   //[UU]
};

class Tx1A0Dh
{
    public:
    const int             actualPosition            = 0x6063 ;  //Counts
    // This position value and positionActualValue can be different, this variable gives
    // number of counts coming from our encoder, it doesn't take gears into account.
    // There is a scalar factor relationship between this value and positionActualValue
    // If we have 10/1 gear, variable coming from this address should be divided by 10 to get
    // exact position of the motor.
    // actualPosition = positionActualValue * GearRatio
};

class Tx1A0Eh
{
    public:
    const int             positionActualValue       = 0x6064 ;  //  Counts
};

class Tx1A0Fh
{
    public:
    const int             velocityActualValue       = 0x6069 ;  // [Counts/sec]
};
class Tx1A10h
{
    public:
    const int             velocityDemand            = 0x606B ;  // [Counts/sec]
};

class Tx1A11h
{
    public:
    const int             velocityActualValue            = 0x606B ;  // [Counts/sec]
};

class Tx1A12h
{
    public:
    const short           torqueDemandValue              = 0x6074 ;  // [Counts/sec]
};

class Tx1A13h
{
    public:
    const short           torqueActualValue              = 0x6077 ;  // [Counts/sec]
};

class Tx1A14h
{
    public:
    const short           touchProbeStatus               = 0x60B9 ;  // [Counts/sec]
};

class Tx1A15h
{
    public:
    const int           touchProbePos1Positive     = 0x60BA ;  
};

class Tx1A16h
{
    public:
    const int           touchProbePos1Negative      = 0x60BB ;  
};

class Tx1A17h
{ 
    public:
    const int           touchProbePos2Positive      = 0x60BC ; 
};

class Tx1A18h
{
    public:
    const int           dcLinkCircuitVoltage          = 0x6079 ;  
};

class Tx1A19h
{
    public:
    const int         positionFollowingError         = 0x60F4 ; 
};

class Tx1A1Ah
{
    public:
    const int           controlEffort                  = 0x60FA ;  //[cnt/sec]
};

class Tx1A1Bh
{
    public:
    const int           positionDemandValue            = 0X60FC ;  //[counts]
};

class Tx1A1Ch
{
    public:
    const int           digitalInputs                 = 0x60FD ;  
};

class Tx1A1Dh
{
    public:
    const short           analogInput                 = 0X2205 ;  
};

class Tx1A1Eh
{
    public:
    const int           auxiliaryPositionValue        = 0x20A0 ;  
};

class Tx1A1Fh
{
    public:
    const short           currentActualValue          = 0x6078 ;  
};

class Tx1A20h
{
    public:
    const int           touchProbePosition2Values     = 0x60BD ;  
};

class Tx1A21h
{ 
    public:
    const int           extraStatusRegister            = 0x2F45 ;  
};

#endif