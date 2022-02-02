

function AsciiMassageParser() {


  this._flush = function() {
    this._needToFlush = false;
    this._receivedDataIndex = 0;
    this._internalString = new Array();
    this._currentWord = 0;
    this._ready = false;
    this._words = null;
  }

  this._flush();

  this.parse = function ( data , callback = null ) {
    
   

    if ( this._needToFlush || this._receivedDataIndex > 1024 ) {
      this._flush();
    }

    if ( data == '\n'  ) {
       
      this._needToFlush = true;
      if ( this._receivedDataIndex > 0 ) {
        this._words = this._internalString.split(" ");
        this._ready = true;
        this._currentWord = 1;
      
        if ( callback != null ) callback();
        return true;
      }
      
    } else {
      this._internalString += data;
      this._receivedDataIndex++;
    }
    return false;
  }


  this._isReady = function() {

    if ( this._ready && this._currentWord < this._words.length ) {
      return true;
    } else {
      return false;
    }
  }

  this.fullMatch = function( address ) {
    if ( this._isReady() &&  this._words[0] == address ) {
      return true;
    }
    return false;
  }

  this.nextByte = function() {

    if ( this._isReady() ) {
      var data =  parseInt(this._words[this._currentWord]);
      this._currentWord++;
      return data;
    }
    return 0;
  }

  this.nextInt = function() {

   if ( this._isReady() ) {
      var data =  parseInt(this._words[this._currentWord]);
      this._currentWord++;
      return data;
    }
    return 0;
  }

  this.nextFloat = function() {

    if ( this._isReady() ) {
      var data =  parseFloat(this._words[this._currentWord]);
      this._currentWord++;
      return data;
    }
    return 0;
  }

  this.nextLong = function() {

     if ( this._isReady() ) {
      var data =  parseInt(this._words[this._currentWord]);
      this._currentWord++;
      return data;
    }
    return 0;
  }

}