
import java.lang.reflect.*;

class AsciiMassageParser {

  java.lang.reflect.Method callback;
  Object instance;
  String callbackName = "";

  byte[] receivedDataArray = new byte[1024];
  int receivedDataIndex = 0;

  String addr;
  String[] words;
  int currentWord = 0;
  boolean ready = false;

  boolean needToFlush = false;


  AsciiMassageParser( Object sketch ) {

    this.instance = sketch;
  }

  private void flush() {
    needToFlush = false;
    receivedDataIndex = 0;
    currentWord = 0;
    ready = false;
  }

  boolean parse( int data ) {
    
    if ( needToFlush || ( receivedDataIndex > receivedDataArray.length ) ) {
      flush();
    }

    if ( data == 10  ) {
      needToFlush = true;
      if ( receivedDataIndex > 0 ) {
        String rawMassage = new String(receivedDataArray, 0, receivedDataIndex);
        words = splitTokens(rawMassage);
        ready = true;
        currentWord = 1;
        
        return true;
      }
      
    } else if ( data > 31 && data < 128) {
      receivedDataArray[receivedDataIndex] = byte(data);
      receivedDataIndex++;
    }
    return false;
  }

  boolean parse( int data, String callbackName) {

    boolean completed =  parse(data);
    if ( completed ) {
      
      if (this.callbackName.equals(callbackName) == false ) {
        this.callbackName = callbackName;
        callback = findCallback(callbackName);
      }
      
      try {
        callback.invoke(instance);
      } 
      catch (ReflectiveOperationException e) {
        print("Dropping massage, could not find callback called "+callbackName);
      }
    }

    return completed;

  }


  private boolean isReady() {

    if ( ready && currentWord < words.length ) {
      return true;
    } else {
      return false;
    }
  }

  boolean fullMatch( String s) {
    if ( isReady() &&  words[0].equals(s) ) {
      return true;
    }
    return false;
  }

  byte nextByte() {

    if ( isReady() ) {
      int data =  int((words[currentWord]));
      currentWord++;
      return (byte)data;
    }
    return 0;
  }

  int nextInt() {

    if ( isReady() ) {
      int data =  int(words[currentWord]);
      currentWord++;
      return data;
    }
    return 0;
  }

  float nextFloat() {

    if ( isReady() ) {
      float data =  float(words[currentWord]);
      currentWord++;
      return data;
    }
    return 0;
  }

  long nextLong() {

    if ( isReady() ) {
      long data =  Long.parseLong(words[currentWord]);
      currentWord++;
      return data;
    }
    return 0;
  }


  private Method findCallback(final String name) {
    try {
      return instance.getClass().getMethod(name);
    } 
    catch (Exception e) {
    }
    // Permit callback(Object) as alternative to callback(Serial).
    try {
      return instance.getClass().getMethod(name);
    } 
    catch (Exception e) {
    }
    return null;
  }
}