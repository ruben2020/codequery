/**
 * @file: num.h 
 * Interface and implementation of numeration functionality.
 *
 * @defgroup Nums Numeration of objects
 *
 * @ingroup GraphBase
 * Numerations can be used to assign numbers to objects.
 *
 * @par
 * In application to graphs a numeration is a mapping of graph's nodes and/or edges to 
 * numbers. A node or an edge can be assigned a number in several different numerations. So
 * when we number them we say what numeration we are dealing with. Numeration class describes
 * one particular numeration. It is used as a parameter for functions like Numbered::number()
 * or Numbered::setNumber()
 *
 * @par
 * Usage model is following. To make an object numerable you derive it from Numbered class.
 * Then if you have some class representing a container for this objects you derive it from
 * the NumManager class. When you want to numerate objects derived from Numbered you ask
 * NumManager-derived object for a new numeration using NumManager::newNum(). Then use
 * Numbered::setNumber() and Numbered::number() to number objects and retrieve object number.
 * When you done with using numbers free the numeration by calling NumManager::freeNum().
 *@code
 //Derive your class from Numbered
 class Obj: public Numbered
 {
 pubic:
     void someAction();
     ...
 }
 //Derive class that controls Obj's instances from NumManager
 class ObjManager: public NumManager
 {
     // QList from Qt's containers
     QList<Obj *> objs;
  
     //Required reimplementation
     void clearNumerationsInObjects()
     {
         // Qt's macros for accessing each object in a container
         foreach ( Obj *obj, objs)
         {
             clearUnusedNumerations( obj);
         }
     }
 public: 
    Obj *newObj(){...}
    QList<Obj *> getObjects(){...};
 }
 //Usage
 void func()
 {
     ObjManager man;
     Numeration num = man.newNum(); // Acquire numeration
     for ( unsigned int i = 0; i < 100; i++)
     {
         Obj* obj = man.newObj();
         obj->setNumber( num, i % 2); // Assign number to object
     }
     foreach ( Obj *obj, man.getObjects())
     {
         if ( obj->number( num)) // Get object's number
            obj->someAction();
     }
     man.freeNum( num); // Free numeration
 }
 @endcode
 * You can have not more than MAX_NUMERATIONS at one time so if you forget to 
 * free a numeration you'll run into exception telling you that there are no free numeration
 * indices left.
 *
 * @sa Mark
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
/**
 * Numeration index type
 * @ingroup Nums
 */
typedef unsigned short int NumIndex;
/**
 * Numeration value type
 * @ingroup Nums
 */
typedef unsigned int NumValue;

/**
 * Possible num errors
 *
 * @ingroup Nums
 */
enum NumErrorType
{
    /** Some error occured */
    NUM_ERROR_GENERIC,
    /** We've ran out of indexes. Someone forgot to free nums? */
    NUM_ERROR_OUT_OF_INDEXES,
    /** We're out of values. Seems to be interanl error of num class.*/
    NUM_ERROR_OUT_OF_VALUES,
    /** Number is too big */
    NUM_ERROR_NUMBER_OUT_OF_RANGE,
    /** Number of error types */
    NUM_ERROR_NUM
};

/* Num-related constants */
/** 
 * Number of numerations allowed simultaneously  
 * @ingroup Nums
 */
const short int MAX_NUMERATIONS = 10;
/** 
 * Clean value
 * @ingroup Nums
 */
const NumValue NUM_VAL_CLEAN = 0;
/** 
 * First value
 * @ingroup Nums
 */
const NumValue NUM_VAL_FIRST = 1;
/**
 * Last value
 * @ingroup Nums
 */
const NumValue NUM_VAL_LAST = ( NumValue)( (int)-1);
/**
 *
 * Value that means object is unnumbered
 * @ingroup Nums
 */
const GraphNum NUMBER_NO_NUM = ( GraphNum) -1;
/**
 * Maximal number of object
 * @ingroup Nums
 */
const GraphNum NUMBER_MAX = (( GraphNum) -1) - 1;

/**
 * @class Numeration
 * @ingroup Nums
 * @brief Numeration description. 
 *
 * @par
 * In application to graphs a numeration is a mapping of graph's nodes and/or edges to 
 * numbers. A node or an edge can be assigned a number in several different numerations. So
 * when we number them we say what numeration we are dealing with. Numeration class describes
 * one particular numeration. It is used as a parameter for functions like Numbered::number()
 * or Numbered::setNumber(). 
 *
 * @par Some Implemetation Details
 * Numeration is created by NumManager class which ensures that numeration's index is used 
 * for this particular numeration only and gives it new value. Value field is used for 
 * comparison in routines like Numbered::isNumbered(). Value in numeration is compared to value
 * stored in @ref Numbered "numbered" object in array position pointed by numeration's index.
 * 
 * @sa Nums
 */
class Numeration
{
public:
    /** Default constructor */
    inline Numeration();
private:
    
    /** Nums index */
    NumIndex index;
    /** Value */
    NumValue value;

    /* Two classes have acces to num internals. All others do not. */
    friend class Numbered;
    friend class NumManager;
};

/** Default constructor */
inline Numeration::Numeration():
    index( MAX_NUMERATIONS), value( NUM_VAL_CLEAN)
{

}

/**
 * @class Numbered
 * @brief Represents an object that can be involved in several numerations
 *
 * @ingroup Nums
 * 
 * To assign numbers to objects you often have to incorporate it into object's implementation or 
 * make some sort of mapping of objects to numbers. The Numbered class can store several numbers
 * simultaneously so it can be member of several numerations at the same time. Being a part of
 * numeration an object can be: numbered or unnumbered. Object becomes numbered when a number is
 * given to it. Until then object is unnumbered. It is also possible to wipe the number from object
 * by calling unNumber(). Most routines have the @ref Numeration "numeration" parameter. It 
 * describes numeration of interest and is obtained from a @ref NumManager "numeration manager".
 */
class Numbered
{
public:
    /** Constructor */
    Numbered();

    /** Assign a number to object. Return false if object is already numbered. True otherwise. */
    inline bool setNumber( Numeration num, GraphNum new_number);
    
    /** Return number in given numeration or NO_NUM if it was not numbered yet */
    inline GraphNum number( Numeration num);
    
    /** Return true if node is numbered in this numeration */
    inline bool isNumbered( Numeration num);
    
    /** Return true if node has been numbered in this numeration and unmarks it */
    inline bool unNumber( Numeration num);
    
    /** Clears value for given index */
    inline void clear( NumIndex i);
private:
    /** Numeration descriptions */
    NumValue nums[ MAX_NUMERATIONS];

    /** Number in each numeration */
    GraphNum numbers[ MAX_NUMERATIONS];
};

/** Constructor */
inline Numbered::Numbered()
{
    NumIndex i;

    /** Initialize nums */
    for ( i = 0; i < MAX_NUMERATIONS; i++)
    {
        nums [ i] = NUM_VAL_CLEAN;
    }
}

/**
 * Assign a number to object. Return false if object is already numbered. True otherwise.
 */
inline bool 
Numbered::setNumber( Numeration num,
                     GraphNum new_number)
{
    if ( new_number >= NUMBER_MAX)
        throw NUM_ERROR_NUMBER_OUT_OF_RANGE;

    nums[ num.index] = num.value;
    numbers[ num.index] = new_number;
    if ( nums[ num.index] == num.value)
    {
        return false;
    }
    return true;
}

/**
 * Return number in given numeration or NO_NUM if it was not numbered yet
 */
inline GraphNum 
Numbered::number( Numeration num)
{
    if ( nums[ num.index] == num.value)
    {
        return numbers[ num.index];
    }
    return NUMBER_NO_NUM;
}

/**
 * Return true if node is numbered in this numeration
 */
inline bool 
Numbered::isNumbered( Numeration num)
{
    if ( nums[ num.index] == num.value)
    {
        return true;
    }
    return false;
}

/**
 * Return true if node has been numbered in this numeration and unmarks it
 */
inline bool 
Numbered::unNumber( Numeration num)
{
    if ( nums[ num.index] == num.value)
    {
        nums[ num.index] = NUM_VAL_CLEAN;
        return true;
    }
    return false;
}
    
/**
 * Clears value for given index
 */
inline void 
Numbered::clear( NumIndex i)
{
       nums[ i] = NUM_VAL_CLEAN;
}

/**
 * @brief Class that creates/frees numerations
 * @ingroup Nums
 *
 * @par
 * @ref NumManager "Numeration manager" creates and frees numerations. You can have up to MAX_NUMERATIONS numerations
 * at a time. A derived class should reimplement clearNumerationsInObjects() routine which calls clearUnusedNumerations
 * for every Numbered-derived object.
 */
class NumManager
{
public:

    /** Default Constructor */
    NumManager();
    
    /**
     * @brief Create new numeration
     * 
     * Create new numeration. Numerations MUST be freed after use,
     * otherwise you run to numerations number limit.
     */
    Numeration newNum();

    /** Free num */
    void freeNum( Numeration n);
protected:
    /** Clears unused markers in given object */
    inline void clearUnusedNumerations( Numbered *n_obj);
private:
    /** Marker values for each numeration */
    NumValue nums[ MAX_NUMERATIONS];
    /** Usage flags for each numeration index */
    bool is_used[ MAX_NUMERATIONS];
    /** Last used value */
    NumValue last;

    /** Find free index */
    inline NumIndex findFreeIndex();
    
    /** Increment num value */
    inline NumValue nextValue();
 
    /** Check if this value is busy */
    inline bool isValueBusy( NumValue val);

    /**
     * @brief Wipe values and indices that are not in use from numbered objects
     *
     * Clear info in objects. MUST BE implemented in inhereted class.
     * Implementation should call clearUnusedNumerations() for every object that can be numbered
     */
    virtual void clearNumerationsInObjects() = 0;

    /** Return next free value */
    inline NumValue findNextFreeValue();
};

/** Implementatinal routines */
/**
 * Find free index
 */
inline NumIndex NumManager::findFreeIndex()
{
    NumIndex i = 0;
    /** Search for free num index */
    for ( i = 0; i < MAX_NUMERATIONS; i++)
    {
        if ( !is_used [ i])
        {
            return i;
        }
    }
    throw NUM_ERROR_OUT_OF_INDEXES;
    return i;
}

/**
 * Increment num value
 */
inline NumValue NumManager::nextValue()
{
    if ( last == NUM_VAL_LAST)
    {
        last = NUM_VAL_FIRST;
    } else
    {
        last++;
    }
    return last;
}

/**
 * Check if this value is busy
 */
inline bool NumManager::isValueBusy( NumValue val)
{
    /** Check all nums */
    for ( NumIndex i = 0; i < MAX_NUMERATIONS; i++)
    {
        if ( is_used [ i] && nums[ i] == val)
            return true;
    }
    return false;
}

/**
 * Return next free value
 */
inline NumValue NumManager::findNextFreeValue()
{
    NumIndex i = 0;
    bool reached_limit = false;
    NumValue res = last;
    
    while( isValueBusy( res))
    {
        /** 
         * If we reached checked NUM_VAL_LAST twice,
         * then we are in infinite loop because for 
         * some reason we don't free our numerations
         */
        if ( res == NUM_VAL_LAST)
        {
            assert< NumErrorType> ( !reached_limit, 
                                       NUM_ERROR_OUT_OF_VALUES);
            clearNumerationsInObjects();
            reached_limit = true;            
        }
        res = nextValue();
    }
    return res;
}

/**
 * Clears unused markers in given object
 */
inline void NumManager::clearUnusedNumerations( Numbered *n_obj)
{
    for ( NumIndex i = 0; i < MAX_NUMERATIONS; i++)
    {
        if ( !is_used [ i])
            n_obj->clear( i);
    }
}

/**
 * Default Constructor
 */
inline NumManager::NumManager()
{
    NumIndex i;

    /** Initialize nums */
    for ( i = 0; i < MAX_NUMERATIONS; i++)
    {
        nums [ i] = NUM_VAL_CLEAN;
        is_used [ i] = false;
    }
    last = NUM_VAL_FIRST;
}

/**
 * Create new numeration
 */
inline Numeration NumManager::newNum()
{
    Numeration new_num;
        
    new_num.index = findFreeIndex();
    is_used[ new_num.index] = true;
    new_num.value = findNextFreeValue();
    nums[ new_num.index] = new_num.value;
    return new_num;
}

/**
 * Free num
 */
inline void NumManager::freeNum( Numeration n)
{
    is_used[ n.index] = false;
}