
function dv_rolloutManager(handlersDefsArray, baseHandler) {
    this.handle = function () {
        var errorsArr = [];

        var handler = chooseEvaluationHandler(handlersDefsArray);
        if (handler) {
            var errorObj = handleSpecificHandler(handler);
            if (errorObj === null) {
                return errorsArr;
            }
            else {
                var debugInfo = handler.onFailure();
                if (debugInfo) {
                    for (var key in debugInfo) {
                        if (debugInfo.hasOwnProperty(key)) {
                            if (debugInfo[key] !== undefined || debugInfo[key] !== null) {
                                errorObj[key] = encodeURIComponent(debugInfo[key]);
                            }
                        }
                    }
                }
                errorsArr.push(errorObj);
            }
        }

        var errorObjHandler = handleSpecificHandler(baseHandler);
        if (errorObjHandler) {
            errorObjHandler['dvp_isLostImp'] = 1;
            errorsArr.push(errorObjHandler);
        }
        return errorsArr;
    };

    function handleSpecificHandler(handler) {
        var request;
        var errorObj = null;

        try {
            request = handler.createRequest();
            if (request && !request.isSev1) {
                var url = request.url || request;
                if (url) {
                    if (!handler.sendRequest(url)) {
                        errorObj = createAndGetError('sendRequest failed.',
                            url,
                            handler.getVersion(),
                            handler.getVersionParamName(),
                            handler.dv_script);
                    }
                } else {
                    errorObj = createAndGetError('createRequest failed.',
                        url,
                        handler.getVersion(),
                        handler.getVersionParamName(),
                        handler.dv_script,
                        handler.dvScripts,
                        handler.dvStep,
                        handler.dvOther
                    );
                }
            }
        }
        catch (e) {
            errorObj = createAndGetError(e.name + ': ' + e.message, request ? (request.url || request) : null, handler.getVersion(), handler.getVersionParamName(), (handler ? handler.dv_script : null));
        }

        return errorObj;
    }

    function createAndGetError(error, url, ver, versionParamName, dv_script, dvScripts, dvStep, dvOther) {
        var errorObj = {};
        errorObj[versionParamName] = ver;
        errorObj['dvp_jsErrMsg'] = encodeURIComponent(error);
        if (dv_script && dv_script.parentElement && dv_script.parentElement.tagName && dv_script.parentElement.tagName == 'HEAD') {
            errorObj['dvp_isOnHead'] = '1';
        }
        if (url) {
            errorObj['dvp_jsErrUrl'] = url;
        }
        if (dvScripts) {
            var dvScriptsResult = '';
            for (var id in dvScripts) {
                if (dvScripts[id] && dvScripts[id].src) {
                    dvScriptsResult += encodeURIComponent(dvScripts[id].src) + ":" + dvScripts[id].isContain + ",";
                }
            }
            
            
            
        }
        return errorObj;
    }

    function chooseEvaluationHandler(handlersArray) {
        var config = window._dv_win.dv_config;
        var index = 0;
        var isEvaluationVersionChosen = false;
        if (config.handlerVersionSpecific) {
            for (var i = 0; i < handlersArray.length; i++) {
                if (handlersArray[i].handler.getVersion() == config.handlerVersionSpecific) {
                    isEvaluationVersionChosen = true;
                    index = i;
                    break;
                }
            }
        }
        else if (config.handlerVersionByTimeIntervalMinutes) {
            var date = config.handlerVersionByTimeInputDate || new Date();
            var hour = date.getUTCHours();
            var minutes = date.getUTCMinutes();
            index = Math.floor(((hour * 60) + minutes) / config.handlerVersionByTimeIntervalMinutes) % (handlersArray.length + 1);
            if (index != handlersArray.length) { 
                isEvaluationVersionChosen = true;
            }
        }
        else {
            var rand = config.handlerVersionRandom || (Math.random() * 100);
            for (var i = 0; i < handlersArray.length; i++) {
                if (rand >= handlersArray[i].minRate && rand < handlersArray[i].maxRate) {
                    isEvaluationVersionChosen = true;
                    index = i;
                    break;
                }
            }
        }

        if (isEvaluationVersionChosen == true && handlersArray[index].handler.isApplicable()) {
            return handlersArray[index].handler;
        }
        else {
            return null;
        }
    }
}

function doesBrowserSupportHTML5Push() {
    "use strict";
    return typeof window.parent.postMessage === 'function' && window.JSON;
}

function dv_GetParam(url, name, checkFromStart) {
    name = name.replace(/[\[]/, "\\\[").replace(/[\]]/, "\\\]");
    var regexS = (checkFromStart ? "(?:\\?|&|^)" : "[\\?&]") + name + "=([^&#]*)";
    var regex = new RegExp(regexS, 'i');
    var results = regex.exec(url);
    if (results == null)
        return null;
    else
        return results[1];
}

function dv_Contains(array, obj) {
    var i = array.length;
    while (i--) {
        if (array[i] === obj) {
            return true;
        }
    }
    return false;
}

function dv_GetDynamicParams(url, prefix) {
    try {
        prefix = (prefix != undefined && prefix != null) ? prefix : 'dvp';
        var regex = new RegExp("[\\?&](" + prefix + "_[^&]*=[^&#]*)", "gi");
        var dvParams = regex.exec(url);

        var results = [];
        while (dvParams != null) {
            results.push(dvParams[1]);
            dvParams = regex.exec(url);
        }
        return results;
    }
    catch (e) {
        return [];
    }
}

function dv_createIframe() {
    var iframe;
    if (document.createElement && (iframe = document.createElement('iframe'))) {
        iframe.name = iframe.id = 'iframe_' + Math.floor((Math.random() + "") * 1000000000000);
        iframe.width = 0;
        iframe.height = 0;
        iframe.style.display = 'none';
        iframe.src = 'about:blank';
    }

    return iframe;
}

function dv_GetRnd() {
    return ((new Date()).getTime() + "" + Math.floor(Math.random() * 1000000)).substr(0, 16);
}

function dv_SendErrorImp(serverUrl, errorsArr) {

    for (var j = 0; j < errorsArr.length; j++) {
        var errorObj = errorsArr[j];
        var errorImp =   dv_CreateAndGetErrorImp(serverUrl, errorObj);
        dv_sendImgImp(errorImp);
    }
}

function dv_CreateAndGetErrorImp(serverUrl, errorObj) {
    var errorQueryString = '';
    for (key in errorObj) {
        if (errorObj.hasOwnProperty(key)) {
            if (key.indexOf('dvp_jsErrUrl') == -1) {
                errorQueryString += '&' + key + '=' + errorObj[key];
            }
            else {
                var params = ['ctx', 'cmp', 'plc', 'sid'];
                for (var i = 0; i < params.length; i++) {
                    var pvalue = dv_GetParam(errorObj[key], params[i]);
                    if (pvalue) {
                        errorQueryString += '&dvp_js' + params[i] + '=' + pvalue;
                    }
                }
            }
        }
    }

    var windowProtocol = 'https:';
    var sslFlag = '&ssl=1';

    var errorImp = windowProtocol + '//' + serverUrl + sslFlag + errorQueryString;
    return errorImp;
}

function dv_getDVUniqueKey(elm) {
    return elm && elm.getAttribute('data-uk');
}

function dv_getDVErrorGlobalScope(elm) {
    var uniqueKey = dv_getDVUniqueKey(elm);
    return uniqueKey && window._dv_win && window._dv_win[uniqueKey] && window._dv_win[uniqueKey].globalScopeVerifyErrorHandler || {};
}

function dv_onLoad(evt) {
    var elm = evt && evt.target || {};
    var globalScope = dv_getDVErrorGlobalScope(elm);
    if (globalScope) {
        var scriptSRC = dv_getScriptSRC(elm);
        if (!globalScope.isJSONPCalled) {
            setTimeout(function onTimeout(){
                globalScope.onTimeout(scriptSRC);
            }, globalScope.msTillJSONPCalled);
        }
    }
}

function dv_onResponse(evt) {
    var elm = evt && evt.target || {};
    var globalScope = dv_getDVErrorGlobalScope(elm);
    if (globalScope) {
        var scriptSRC = dv_getScriptSRC(elm);
        if (!globalScope.isJSONPCalled) {
            globalScope.onResponse(scriptSRC);
        }
    }
}

function dv_getScriptSRC(elm) {
    return elm && elm.src || '';
}
var IQPAParams = [
    "auprice", "ppid", "audeal", "auevent", "auadv", "aucmp", "aucrtv", "auorder", "ausite", "auplc", "auxch", "audvc", "aulitem",
    "auadid", "pltfrm", "aufilter1", "aufilter2", "autt", "auip", "aubndl", "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8", "c9",
    "c10", "c11", "c12", "c13", "c14", "c15"
];

function dv_AppendIQPAParams(src) {
    var qs = [];
    var paramVal;
    IQPAParams.forEach(function forEachParam(paramName){
        paramVal = dv_GetParam(src, paramName);
        if (paramVal !== '' && paramVal !== null) {
            qs.push([paramName, paramVal].join('='));
        }
    });
    return qs.length && '&' + qs.join('&') || '';
}

function dv_onError(evt) {
    var elm = evt && evt.target || {};
    var globalScope = dv_getDVErrorGlobalScope(elm);
    if (globalScope) {
        globalScope.onError(dv_getScriptSRC(elm));
    }
}

function dv_getDVBSErrAddress(config) {
    return config && config.bsErrAddress || 'rtb0.doubleverify.com';
}

function dv_sendImgImp(url) {
    (new Image()).src = url;
}

function dv_sendScriptRequest(url, onLoad, onError, uniqueKey) {
    var emptyFunction = function(){};
    onLoad = onLoad || emptyFunction;
    onError = onError || emptyFunction;
    document.write('<scr' + 'ipt data-uk="' + uniqueKey + '" onerror="(' + onError + ')({target:this});" onload="(' + onLoad + ')({target:this});" type="text/javascript" src="' + url + '"></scr' + 'ipt>');
}

function dv_getPropSafe(obj, propName) {
    try {
        if (obj)
            return obj[propName];
    } catch (e) {
    }
}

function dvBsType() {
    var that = this;
    var eventsForDispatch = {};
    this.t2tEventDataZombie = {};

    this.getEventsForDispatch = function getEventsForDispatch () {
        return eventsForDispatch;
    }; 

    this.processT2TEvent = function (data, tag) {
        try {
            if (tag.ServerPublicDns) {
                data.timeStampCollection.push({"beginProcessT2TEvent": getCurrentTime()});
                data.timeStampCollection.push({'beginVisitCallback': tag.beginVisitCallbackTS});
                var tpsServerUrl = tag.dv_protocol + '//' + tag.ServerPublicDns + '/event.gif?impid=' + tag.uid;

                if (!tag.uniquePageViewId) {
                    tag.uniquePageViewId = data.uniquePageViewId;
                }

                tpsServerUrl += '&dvp_upvid=' + tag.uniquePageViewId;
                tpsServerUrl += '&dvp_numFrames=' + data.totalIframeCount;
                tpsServerUrl += '&dvp_numt2t=' + data.totalT2TiframeCount;
                tpsServerUrl += '&dvp_frameScanDuration=' + data.scanAllFramesDuration;
                tpsServerUrl += '&dvp_scene=' + tag.adServingScenario;
                tpsServerUrl += '&dvp_ist2twin=' + (data.isWinner ? '1' : '0');
                tpsServerUrl += '&dvp_numTags=' + Object.keys($dvbs.tags).length;
                tpsServerUrl += '&dvp_isInSample=' + data.isInSample;
                tpsServerUrl += (data.wasZombie) ? '&dvp_wasZombie=1' : '&dvp_wasZombie=0';
                tpsServerUrl += '&dvp_ts_t2tCreatedOn=' + data.creationTime;
                if (data.timeStampCollection) {
                    if (window._dv_win.t2tTimestampData) {
                        for (var tsI = 0; tsI < window._dv_win.t2tTimestampData.length; tsI++) {
                            data.timeStampCollection.push(window._dv_win.t2tTimestampData[tsI]);
                        }
                    }

                    for (var i = 0; i < data.timeStampCollection.length; i++) {
                        var item = data.timeStampCollection[i];
                        for (var propName in item) {
                            if (item.hasOwnProperty(propName)) {
                                tpsServerUrl += '&dvp_ts_' + propName + '=' + item[propName];
                            }
                        }
                    }
                }
                $dvbs.domUtilities.addImage(tpsServerUrl, tag.tagElement.parentElement);
            }
        } catch (e) {
            try {
                dv_SendErrorImp(window._dv_win.dv_config.tpsErrAddress + '/visit.jpg?ctx=818052&cmp=1619415&dvtagver=6.1.src&jsver=0&dvp_ist2tProcess=1', {dvp_jsErrMsg: encodeURIComponent(e)});
            } catch (ex) {
            }
        }
    };

    this.processTagToTagCollision = function (collision, tag) {
        var i;
        var tpsServerUrl = tag.dv_protocol + '//' + tag.ServerPublicDns + '/event.gif?impid=' + tag.uid;
        var additions = [
            '&dvp_collisionReasons=' + collision.reasonBitFlag,
            '&dvp_ts_reporterDvTagCreated=' + collision.thisTag.dvTagCreatedTS,
            '&dvp_ts_reporterVisitJSMessagePosted=' + collision.thisTag.visitJSPostMessageTS,
            '&dvp_ts_reporterReceivedByT2T=' + collision.thisTag.receivedByT2TTS,
            '&dvp_ts_collisionPostedFromT2T=' + collision.postedFromT2TTS,
            '&dvp_ts_collisionReceivedByCommon=' + collision.commonRecievedTS,
            '&dvp_collisionTypeId=' + collision.allReasonsForTagBitFlag
        ];
        tpsServerUrl += additions.join("");

        for (i = 0; i < collision.reasons.length; i++) {
            var reason = collision.reasons[i];
            tpsServerUrl += '&dvp_' + reason + "MS=" + collision[reason + "MS"];
        }

        if (tag.uniquePageViewId) {
            tpsServerUrl += '&dvp_upvid=' + tag.uniquePageViewId;
        }
        $dvbs.domUtilities.addImage(tpsServerUrl, tag.tagElement.parentElement);
    };

    var messageEventListener = function (event) {
        try {
            var timeCalled = getCurrentTime();
            var data = window.JSON.parse(event.data);
            if (!data.action) {
                data = window.JSON.parse(data);
            }
            if (data.timeStampCollection) {
                data.timeStampCollection.push({messageEventListenerCalled: timeCalled});
            }
            var myUID;
            var visitJSHasBeenCalledForThisTag = false;
            if ($dvbs.tags) {
                for (var uid in $dvbs.tags) {
                    if ($dvbs.tags.hasOwnProperty(uid) && $dvbs.tags[uid] && $dvbs.tags[uid].t2tIframeId === data.iFrameId) {
                        myUID = uid;
                        visitJSHasBeenCalledForThisTag = true;
                        break;
                    }
                }
            }

            switch (data.action) {
                case 'uniquePageViewIdDetermination' :
                    if (visitJSHasBeenCalledForThisTag) {
                        $dvbs.processT2TEvent(data, $dvbs.tags[myUID]);
                        $dvbs.t2tEventDataZombie[data.iFrameId] = undefined;
                    }
                    else {
                        data.wasZombie = 1;
                        $dvbs.t2tEventDataZombie[data.iFrameId] = data;
                    }
                    break;
                case 'maColl':
                    var tag = $dvbs.tags[myUID];
                    
                    tag.AdCollisionMessageRecieved = true;
                    if (!tag.uniquePageViewId) {
                        tag.uniquePageViewId = data.uniquePageViewId;
                    }
                    data.collision.commonRecievedTS = timeCalled;
                    $dvbs.processTagToTagCollision(data.collision, tag);
                    break;
            }

        } catch (e) {
            try {
                dv_SendErrorImp(window._dv_win.dv_config.tpsErrAddress + '/visit.jpg?ctx=818052&cmp=1619415&dvtagver=6.1.src&jsver=0&dvp_ist2tListener=1', {dvp_jsErrMsg: encodeURIComponent(e)});
            } catch (ex) {
            }
        }
    };

    if (window.addEventListener)
        addEventListener("message", messageEventListener, false);
    else
        attachEvent("onmessage", messageEventListener);

    this.pubSub = new function () {

        var subscribers = [];

        this.subscribe = function (eventName, uid, actionName, func) {
            if (!subscribers[eventName + uid])
                subscribers[eventName + uid] = [];
            subscribers[eventName + uid].push({Func: func, ActionName: actionName});
        };

        this.publish = function (eventName, uid) {
            var actionsResults = [];
            if (eventName && uid && subscribers[eventName + uid] instanceof Array)
                for (var i = 0; i < subscribers[eventName + uid].length; i++) {
                    var funcObject = subscribers[eventName + uid][i];
                    if (funcObject && funcObject.Func && typeof funcObject.Func == "function" && funcObject.ActionName) {
                        var isSucceeded = runSafely(function () {
                            return funcObject.Func(uid);
                        });
                        actionsResults.push(encodeURIComponent(funcObject.ActionName) + '=' + (isSucceeded ? '1' : '0'));
                    }
                }
            return actionsResults.join('&');
        };
    };

    this.domUtilities = new function () {

        this.addImage = function (url, parentElement, trackingPixelCompleteCallbackName) {
            url = appendCacheBuster(url);
            if (typeof(navigator.sendBeacon) === 'function') {
                var isSuccessfullyQueuedDataForTransfer = navigator.sendBeacon(url);
                if (isSuccessfullyQueuedDataForTransfer && typeof(window[trackingPixelCompleteCallbackName]) === 'function') {
                    window[trackingPixelCompleteCallbackName]();
                }
                return;
            }
        
            var image = new Image();
            if (typeof(window[trackingPixelCompleteCallbackName]) === 'function') {
                image.addEventListener('load', window[trackingPixelCompleteCallbackName]);
            }
            image.src = url;
        };

        this.addScriptResource = function (url, parentElement, onLoad, onError, uniqueKey) {
            var emptyFunction = function(){};
            onLoad = onLoad || emptyFunction;
            onError = onError || emptyFunction;
            uniqueKey = uniqueKey || '';
            if (parentElement) {
                var scriptElem = parentElement.ownerDocument.createElement("script");
                scriptElem.onerror = onError;
                scriptElem.onload = onLoad;
                if (scriptElem && typeof(scriptElem.setAttribute) === 'function') {
                    scriptElem.setAttribute('data-uk', uniqueKey);
                }
                scriptElem.type = 'text/javascript';
                scriptElem.src = appendCacheBuster(url);
                parentElement.insertBefore(scriptElem, parentElement.firstChild);
            }
            else {
                addScriptResourceFallBack(url, onLoad, onError, uniqueKey);
            }
        };

        function addScriptResourceFallBack(url, onLoad, onError, uniqueKey) {
            var emptyFunction = function(){};
            onLoad = onLoad || emptyFunction;
            onError = onError || emptyFunction;
            uniqueKey = uniqueKey || '';
            var scriptElem = document.createElement('script');
            scriptElem.onerror = onError;
            scriptElem.onload = onLoad;
            if (scriptElem && typeof(scriptElem.setAttribute) === 'function') {
                scriptElem.setAttribute('data-uk', uniqueKey);
            }
            scriptElem.type = "text/javascript";
            scriptElem.src = appendCacheBuster(url);
            var firstScript = document.getElementsByTagName('script')[0];
            firstScript.parentNode.insertBefore(scriptElem, firstScript);
        }

        this.addScriptCode = function (srcCode, parentElement) {
            var scriptElem = parentElement.ownerDocument.createElement("script");
            scriptElem.type = 'text/javascript';
            scriptElem.innerHTML = srcCode;
            parentElement.insertBefore(scriptElem, parentElement.firstChild);
        };

        this.addHtml = function (srcHtml, parentElement) {
            var divElem = parentElement.ownerDocument.createElement("div");
            divElem.style = "display: inline";
            divElem.innerHTML = srcHtml;
            parentElement.insertBefore(divElem, parentElement.firstChild);
        };
    };

    this.resolveMacros = function (str, tag) {
        var viewabilityData = tag.getViewabilityData();
        var viewabilityBuckets = viewabilityData && viewabilityData.buckets ? viewabilityData.buckets : {};
        var upperCaseObj = objectsToUpperCase(tag, viewabilityData, viewabilityBuckets);
        var newStr = str.replace('[DV_PROTOCOL]', upperCaseObj.DV_PROTOCOL);
        newStr = newStr.replace('[PROTOCOL]', upperCaseObj.PROTOCOL);
        newStr = newStr.replace(/\[(.*?)\]/g, function (match, p1) {
            var value = upperCaseObj[p1];
            if (value === undefined || value === null)
                value = '[' + p1 + ']';
            return encodeURIComponent(value);
        });
        return newStr;
    };

    this.settings = new function () {
    };

    this.tagsType = function () {
    };

    this.tagsPrototype = function () {
        this.add = function (tagKey, obj) {
            if (!that.tags[tagKey])
                that.tags[tagKey] = new that.tag();
            for (var key in obj)
                that.tags[tagKey][key] = obj[key];
        };
    };

    this.tagsType.prototype = new this.tagsPrototype();
    this.tagsType.prototype.constructor = this.tags;
    this.tags = new this.tagsType();

    this.tag = function () {
    };
    this.tagPrototype = function () {
        this.set = function (obj) {
            for (var key in obj)
                this[key] = obj[key];
        };

        this.getViewabilityData = function () {
        };
    };

    this.tag.prototype = new this.tagPrototype();
    this.tag.prototype.constructor = this.tag;

    this.getTagObjectByService = function (serviceName) {

        for (var impressionId in this.tags) {
            if (typeof this.tags[impressionId] === 'object'
                && this.tags[impressionId].services
                && this.tags[impressionId].services[serviceName]
                && !this.tags[impressionId].services[serviceName].isProcessed) {
                this.tags[impressionId].services[serviceName].isProcessed = true;
                return this.tags[impressionId];
            }
        }


        return null;
    };

    this.addService = function (impressionId, serviceName, paramsObject) {

        if (!impressionId || !serviceName)
            return;

        if (!this.tags[impressionId])
            return;
        else {
            if (!this.tags[impressionId].services)
                this.tags[impressionId].services = {};

            this.tags[impressionId].services[serviceName] = {
                params: paramsObject,
                isProcessed: false
            };
        }
    };

    this.Enums = {
        BrowserId: {Others: 0, IE: 1, Firefox: 2, Chrome: 3, Opera: 4, Safari: 5},
        TrafficScenario: {OnPage: 1, SameDomain: 2, CrossDomain: 128}
    };

    this.CommonData = {};

    var runSafely = function (action) {
        try {
            var ret = action();
            return ret !== undefined ? ret : true;
        } catch (e) {
            return false;
        }
    };

    var objectsToUpperCase = function () {
        var upperCaseObj = {};
        for (var i = 0; i < arguments.length; i++) {
            var obj = arguments[i];
            for (var key in obj) {
                if (obj.hasOwnProperty(key)) {
                    upperCaseObj[key.toUpperCase()] = obj[key];
                }
            }
        }
        return upperCaseObj;
    };

    var appendCacheBuster = function (url) {
        if (url !== undefined && url !== null && url.match("^http") == "http") {
            if (url.indexOf('?') !== -1) {
                if (url.slice(-1) == '&')
                    url += 'cbust=' + dv_GetRnd();
                else
                    url += '&cbust=' + dv_GetRnd();
            }
            else
                url += '?cbust=' + dv_GetRnd();
        }
        return url;
    };

    
    var messagesClass = function () {
        var waitingMessages = [];

        this.registerMsg = function(dvFrame, data) {
            if (!waitingMessages[dvFrame.$frmId]) {
                waitingMessages[dvFrame.$frmId] = [];
            }

            waitingMessages[dvFrame.$frmId].push(data);

            if (dvFrame.$uid) {
                sendWaitingEventsForFrame(dvFrame, dvFrame.$uid);
            }
        };

        this.startSendingEvents = function(dvFrame, impID) {
            sendWaitingEventsForFrame(dvFrame, impID);
            
        };

        function sendWaitingEventsForFrame(dvFrame, impID) {
            if (waitingMessages[dvFrame.$frmId]) {
                var eventObject = {};
                for (var i = 0; i < waitingMessages[dvFrame.$frmId].length; i++) {
                    var obj = waitingMessages[dvFrame.$frmId].pop();
                    for (var key in obj) {
                        if (typeof obj[key] !== 'function' && obj.hasOwnProperty(key)) {
                            eventObject[key] = obj[key];
                        }
                    }
                }
                that.registerEventCall(impID, eventObject);
            }
        }

        function startMessageManager() {
            for (var frm in waitingMessages) {
                if (frm && frm.$uid) {
                    sendWaitingEventsForFrame(frm, frm.$uid);
                }
            }
            setTimeout(startMessageManager, 10);
        }
    };
    this.messages = new messagesClass();

    this.dispatchRegisteredEventsFromAllTags = function () {
        for (var impressionId in this.tags) {
            if (typeof this.tags[impressionId] !== 'function' && typeof this.tags[impressionId] !== 'undefined')
                dispatchEventCalls(impressionId, this);
        }
    };

    var dispatchEventCalls = function (impressionId, dvObj) {
        var tag = dvObj.tags[impressionId];
        var eventObj = eventsForDispatch[impressionId];
        if (typeof eventObj !== 'undefined' && eventObj != null) {
            var url = tag.protocol + '//' + tag.ServerPublicDns + "/bsevent.gif?impid=" + impressionId + '&' + createQueryStringParams(eventObj);
            dvObj.domUtilities.addImage(url, tag.tagElement.parentElement);
            eventsForDispatch[impressionId] = null;
        }
    };

    this.registerEventCall = function (impressionId, eventObject, timeoutMs) {
        addEventCallForDispatch(impressionId, eventObject);

        if (typeof timeoutMs === 'undefined' || timeoutMs == 0 || isNaN(timeoutMs))
            dispatchEventCallsNow(this, impressionId, eventObject);
        else {
            if (timeoutMs > 2000)
                timeoutMs = 2000;

            var dvObj = this;
            setTimeout(function () {
                dispatchEventCalls(impressionId, dvObj);
            }, timeoutMs);
        }
    };

    this.createEventCallUrl = function(impId, eventObject) {
        var tag = this.tags && this.tags[impId];
        if (tag && typeof eventObject !== 'undefined' && eventObject !== null) {
            return [tag.protocol, '//', tag.ServerPublicDns, '/bsevent.gif?impid=', impId, '&', createQueryStringParams(eventObject)].join('');
        }
    }

    var dispatchEventCallsNow = function (dvObj, impressionId, eventObject) {
        addEventCallForDispatch(impressionId, eventObject);
        dispatchEventCalls(impressionId, dvObj);
    };

    var addEventCallForDispatch = function (impressionId, eventObject) {
        for (var key in eventObject) {
            if (typeof eventObject[key] !== 'function' && eventObject.hasOwnProperty(key)) {
                if (!eventsForDispatch[impressionId])
                    eventsForDispatch[impressionId] = {};
                eventsForDispatch[impressionId][key] = eventObject[key];
            }
        }
    };

    if (window.addEventListener) {
        window.addEventListener('unload', function () {
            that.dispatchRegisteredEventsFromAllTags();
        }, false);
        window.addEventListener('beforeunload', function () {
            that.dispatchRegisteredEventsFromAllTags();
        }, false);
    }
    else if (window.attachEvent) {
        window.attachEvent('onunload', function () {
            that.dispatchRegisteredEventsFromAllTags();
        }, false);
        window.attachEvent('onbeforeunload', function () {
            that.dispatchRegisteredEventsFromAllTags();
        }, false);
    }
    else {
        window.document.body.onunload = function () {
            that.dispatchRegisteredEventsFromAllTags();
        };
        window.document.body.onbeforeunload = function () {
            that.dispatchRegisteredEventsFromAllTags();
        };
    }

    var createQueryStringParams = function (values) {
        var params = '';
        for (var key in values) {
            if (typeof values[key] !== 'function') {
                var value = encodeURIComponent(values[key]);
                if (params === '')
                    params += key + '=' + value;
                else
                    params += '&' + key + '=' + value;
            }
        }

        return params;
    };
}

function dv_baseHandler(){function L(b){var c=window._dv_win,e=0;try{for(;10>e;){if(c[b]&&"object"===typeof c[b])return!0;if(c==c.parent)break;e++;c=c.parent}}catch(d){}return!1}function M(){var b="http:";"http:"!=window._dv_win.location.protocol&&(b="https:");return b}function F(b,c){var e=document.createElement("iframe");e.name=window._dv_win.dv_config.emptyIframeID||"iframe_"+B();e.width=0;e.height=0;e.id=c;e.style.display="none";e.src=b;return e}function C(b,c,e){e=e||150;var d=window._dv_win||
window;if(d.document&&d.document.body)return c&&c.parentNode?c.parentNode.insertBefore(b,c):d.document.body.insertBefore(b,d.document.body.firstChild),!0;if(0<e)setTimeout(function(){C(b,c,--e)},20);else return!1}function N(b){var c=window._dv_win.dv_config=window._dv_win.dv_config||{};c.cdnAddress=c.cdnAddress||"cdn.doubleverify.com";return'<html><head><script type="text/javascript">('+function(){try{window.$dv=window.$dvbs||parent.$dvbs,window.$dv.dvObjType="dvbs"}catch(e){}}.toString()+')();\x3c/script></head><body><script type="text/javascript">('+
(b||"function() {}")+')("'+c.cdnAddress+'");\x3c/script><script type="text/javascript">setTimeout(function() {document.close();}, 0);\x3c/script></body></html>'}function G(b){var c=0,e;for(e in b)b.hasOwnProperty(e)&&++c;return c}function O(b,c){a:{var e={};try{if(b&&b.performance&&b.performance.getEntries){var d=b.performance.getEntries();for(b=0;b<d.length;b++){var f=d[b],n=f.name.match(/.*\/(.+?)\./);if(n&&n[1]){var l=n[1].replace(/\d+$/,""),k=c[l];if(k){for(var h=0;h<k.stats.length;h++){var q=
k.stats[h];e[k.prefix+q.prefix]=Math.round(f[q.name])}delete c[l];if(!G(c))break}}}}var p=e;break a}catch(A){}p=void 0}if(p&&G(p))return p}function P(b,c){var e,d=window._dv_win.document.visibilityState;window[b.tagObjectCallbackName]=function(f){var n=window._dv_win.$dvbs;if(n){var l=c?"https:":M();e=f.ImpressionID;n.tags.add(f.ImpressionID,b);n.tags[f.ImpressionID].set({tagElement:b.script,impressionId:f.ImpressionID,dv_protocol:b.protocol,protocol:l,uid:b.uid,serverPublicDns:f.ServerPublicDns,
ServerPublicDns:f.ServerPublicDns});b.script&&b.script.dvFrmWin&&(b.script.dvFrmWin.$uid=f.ImpressionID,n.messages&&n.messages.startSendingEvents&&n.messages.startSendingEvents(b.script.dvFrmWin,f.ImpressionID));(function(){function b(){var e=window._dv_win.document.visibilityState;"prerender"===d&&"prerender"!==e&&"unloaded"!==e&&(d=e,window._dv_win.$dvbs.registerEventCall(f.ImpressionID,{prndr:0}),window._dv_win.document.removeEventListener(c,b))}if("prerender"===d)if("prerender"!==window._dv_win.document.visibilityState&&
"unloaded"!==visibilityStateLocal)window._dv_win.$dvbs.registerEventCall(f.ImpressionID,{prndr:0});else{var c;"undefined"!==typeof window._dv_win.document.hidden?c="visibilitychange":"undefined"!==typeof window._dv_win.document.mozHidden?c="mozvisibilitychange":"undefined"!==typeof window._dv_win.document.msHidden?c="msvisibilitychange":"undefined"!==typeof window._dv_win.document.webkitHidden&&(c="webkitvisibilitychange");window._dv_win.document.addEventListener(c,b,!1)}})()}if("1"!=b.foie)try{var k=
O(window,{verify:{prefix:"vf",stats:[{name:"duration",prefix:"dur"}]}});k&&window._dv_win.$dvbs.registerEventCall(f.ImpressionID,k)}catch(h){}};window[b.callbackName]=function(f){u.setIsJSONPCalled(!0);var c=window._dv_win.$dvbs&&"object"==typeof window._dv_win.$dvbs.tags[e]?window._dv_win.$dvbs.tags[e]:b;var d=window._dv_win.dv_config.bs_renderingMethod||function(b){document.write(b)};switch(f.ResultID){case 1:c.tagPassback?d(c.tagPassback):f.Passback?d(decodeURIComponent(f.Passback)):f.AdWidth&&
f.AdHeight&&d(decodeURIComponent("%3Cdiv%20style%3D%22display%3A%20flex%3B%20align-items%3A%20center%3B%20justify-content%3A%20center%3B%20width%3A%20"+f.AdWidth+"px%3B%20height%3A%20"+f.AdHeight+"px%3B%20outline-offset%3A%20-1px%3B%20background%3A%20url('data%3Aimage%2Fpng%3Bbase64%2CiVBORw0KGgoAAAANSUhEUgAAADoAAAA6CAYAAAGWvHq%2BAAAABmJLR0QA%2FwD%2FAP%2BgvaeTAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH5AQBECEbFuFN7gAAAB1pVFh0Q29tbWVudAAAAAAAQ3JlYXRlZCB3aXRoIEdJTVBkLmUHAAAFBklEQVRo3uUby3arOEzxAbxIyKLt%2F%2F9gIQtIF4aFZ5ERVxhJyIbcnjmjTZLast4vQ%2BG762OMMX53fQzTFIfxGenfKvgXvj4%2FoOsfy3eECgBgmmcYhnFZ6PoHeO%2F%2FbBiGEQAAxufPghlC%2BLOBYqa%2FHezAJcYYOUz87QAA7vd2g4lMAsrLfQJ%2BQeUM43PZsMJEwN8L58gMfgIAAMVKv6syX4bxGVF9qTiuvV2Byouf7e0Kl%2B%2Buj6kJU8aktV07aFClTkThfm9hGMbNGu53dCNd%2FPr8gBCm5TsnAivz%2BPwBqkHvPaDiVvpAW6Nh0FBhmpagSdfQV0Q7oVySPrz3LyO3t%2BvCKrJIHTtdG58%2FvLycZk%2Bzr1uFkgFWuYHKZHHNEMIr4lMb0pO5v7e3qyyj983KATYydv1jswFZneZ5wzaKVaEMVnNgjsw2B8pcbMdLmKbY1PVG5dTl0rVpnsGlSDReOcfo%2Bgc0df3SagrTPC8m4aDrH1ClaR4AgHKRmgN%2FL9HBbeI4wdKVitXUtYpLGXPSgpUg1lBaPzWCWW6wJ4lkB9aFUL1pQkXOvW9WBDltULNM8wwhTEtIcQn88t31kdpEU7FmOwsemqiiqtPsQvufXMCmCulUSKy9XaG9XYGrLhbv1iSlWU0NGdyQqlPKBHQfh0vxVkQ1abSQybX3oQ7nUPWUpEQ1oaokLVAnSfG4cy8xxpjrEFyVtuCJNt3rETDgu%2F6xiT9zRqKSci0DxzHdZ5E0zXabjGTtwSxr9FyqjazSJkmTi%2Bckb01BS5HaGnems%2BZWzdb62qQTfQdwDDl2Wj0RuKnYpX1sDrJljcvHTqow4%2FNn5SBNXYuzPD0Y8agDsRlpr3NIg1vyYGnSS%2BPUURVIcRhC2A0ZyYPxTKqNyuo8IYRlpMSGLYRJDRdOYyEEqEpDIIfY5qYhhLBrL0s%2BLS7imqq995tijYVdCxlx0EMnaW9XlvD93m4aZ0s4cZ3gqspYOjppRKcMcXipGZyU7Ju63iXIhVOKx53trCWqtMpwZzor8n%2BqynBnnlJlNGa5M51VSmlksBSDlOHlKk%2FzUq0KcVVEYgidytz3coS19lPrFh1y2fUP1Xu1HKsRxHWakao9hLNglZHeESaal3vvocKx3zKP7BXnLJtaxgNkjKY1Wp1y7inYUVG7Akg79vSeKefKwHJ1kEtTikBxJrYkmpIBr1TgPdgbrZ1WkPbuz84UEiNZG1ZLhdydE0sqeqlytGG2pEt4%2B0Ccc9H8zs4kS1Br0542F0fqR0lesOCwyehoIioZq86gqcWq6XbZwrTGqMSAhmOhKWVpjp74PObIsLt3R3g0g1oETs8R32woFbLEHUuEs9CiZa6SslZJmpcuf%2F4GcNc0tDf9lYcxvwGVrI3mkDVeY0NjbumOui9XCtkYlZJIbjt3pF8tzQ0czZTvTXnJSdlHSstRXAlPUpQ4vRy1TK4nnNEwaDTd2ZNE6fQSQiieevBiprjXLamjpco5Mv1YSuH%2Fpry4o%2BMPN70cgZI4tYyG7h3J4evzI1tJ%2BIynBLTHMdnlpXQKsTQCkoAaPakZEctL%2BpbK0Y7FMkloCnrXHMsKileMpS0ZR3zvveez2kDJG6szRiSuJqaulfbOaQJ5KfcYH5wnLK82v2uMCmHaPDz%2BDVj%2BfSNNBGdZmIu9v6EIKWbVZHTmVYrl9clSRVsS0urOKDdlW1J%2B6SubFoH3SiF13X8A3uobUgsAG3MAAAAASUVORK5CYII%3D')%20repeat%3B%20outline%3A%20solid%201px%20%23969696%3B%22%3E%3C%2Fdiv%3E"));
break;case 2:case 3:c.tagAdtag&&d(c.tagAdtag);break;case 4:f.AdWidth&&f.AdHeight&&d(decodeURIComponent("%3Cstyle%3E%0A.dvbs_container%20%7B%0A%09border%3A%201px%20solid%20%233b599e%3B%0A%09overflow%3A%20hidden%3B%0A%09filter%3A%20progid%3ADXImageTransform.Microsoft.gradient(startColorstr%3D%27%23315d8c%27%2C%20endColorstr%3D%27%2384aace%27)%3B%0A%7D%0A%3C%2Fstyle%3E%0A%3Cdiv%20class%3D%22dvbs_container%22%20style%3D%22width%3A%20"+f.AdWidth+"px%3B%20height%3A%20"+f.AdHeight+"px%3B%22%3E%09%0A%3C%2Fdiv%3E"))}}}
function Q(b){var c=null,e=null,d=function(b){var f=dv_GetParam(b,"cmp");b=dv_GetParam(b,"ctx");return"919838"==b&&"7951767"==f||"919839"==b&&"7939985"==f||"971108"==b&&"7900229"==f||"971108"==b&&"7951940"==f?"</scr'+'ipt>":/<\/scr\+ipt>/g}(b.src);"function"!==typeof String.prototype.trim&&(String.prototype.trim=function(){return this.replace(/^\s+|\s+$/g,"")});var f=function(b){!(b=b.previousSibling)||"#text"!=b.nodeName||null!=b.nodeValue&&void 0!=b.nodeValue&&0!=b.nodeValue.trim().length||(b=b.previousSibling);
if(b&&"SCRIPT"==b.tagName&&b.getAttribute("type")&&("text/adtag"==b.getAttribute("type").toLowerCase()||"text/passback"==b.getAttribute("type").toLowerCase())&&""!=b.innerHTML.trim()){if("text/adtag"==b.getAttribute("type").toLowerCase())return c=b.innerHTML.replace(d,"\x3c/script>"),{isBadImp:!1,hasPassback:!1,tagAdTag:c,tagPassback:e};if(null!=e)return{isBadImp:!0,hasPassback:!1,tagAdTag:c,tagPassback:e};e=b.innerHTML.replace(d,"\x3c/script>");b=f(b);b.hasPassback=!0;return b}return{isBadImp:!0,
hasPassback:!1,tagAdTag:c,tagPassback:e}};return f(b)}function H(b,c,e,d,f,n,l,k,h,q,p,A){c.dvregion=0;var r=dv_GetParam(m,"useragent");m=window._dv_win.$dvbs.CommonData;if(void 0!=m.BrowserId&&void 0!=m.BrowserVersion&&void 0!=m.BrowserIdFromUserAgent)var t={ID:m.BrowserId,version:m.BrowserVersion,ID_UA:m.BrowserIdFromUserAgent};else t=R(r?decodeURIComponent(r):navigator.userAgent),m.BrowserId=t.ID,m.BrowserVersion=t.version,m.BrowserIdFromUserAgent=t.ID_UA;var g="";void 0!=c.aUrl&&(g="&aUrl="+c.aUrl);
var y="";try{d.depth=S(d);var w=T(d,e,t);if(w&&w.duration){var u="&dvp_strhd="+w.duration;u+="&dvpx_strhd="+w.duration}w&&w.url||(w=U(d));w&&w.url&&(g="&aUrl="+encodeURIComponent(w.url),y="&aUrlD="+w.depth);var x=d.depth+f;n&&d.depth--}catch(I){u=y=g=x=d.depth=""}a:{try{if("object"==typeof window.$ovv||"object"==typeof window.parent.$ovv){var v=1;break a}}catch(I){}v=0}f=function(){function b(f){c++;var d=f.parent==f;return f.mraid||d?f.mraid:20>=c&&b(f.parent)}var f=window._dv_win||window,c=0;try{return b(f)}catch(ea){}}();
var m=c.script.src;v="&ctx="+(dv_GetParam(m,"ctx")||"")+"&cmp="+(dv_GetParam(m,"cmp")||"")+"&plc="+(dv_GetParam(m,"plc")||"")+"&sid="+(dv_GetParam(m,"sid")||"")+"&advid="+(dv_GetParam(m,"advid")||"")+"&adsrv="+(dv_GetParam(m,"adsrv")||"")+"&unit="+(dv_GetParam(m,"unit")||"")+"&isdvvid="+(dv_GetParam(m,"isdvvid")||"")+"&uid="+c.uid+"&tagtype="+(dv_GetParam(m,"tagtype")||"")+"&adID="+(dv_GetParam(m,"adID")||"")+"&app="+(dv_GetParam(m,"app")||"")+"&sup="+(dv_GetParam(m,"sup")||"")+"&isovv="+v+"&gmnpo="+
(dv_GetParam(m,"gmnpo")||"")+"&crt="+(dv_GetParam(m,"crt")||"");"1"==dv_GetParam(m,"foie")&&(v+="&foie=1");f&&(v+="&ismraid=1");(f=dv_GetParam(m,"xff"))&&(v+="&xff="+f);(f=dv_GetParam(m,"vssd"))&&(v+="&vssd="+f);(f=dv_GetParam(m,"apifw"))&&(v+="&apifw="+f);(f=dv_GetParam(m,"vstvr"))&&(v+="&vstvr="+f);(f=dv_GetParam(m,"tvcp"))&&(v+="&tvcp="+f);p&&(v+="&urlsrc=sf");A&&(v+="&sfe=1");navigator&&navigator.maxTouchPoints&&5==navigator.maxTouchPoints&&(v+="&touch=1");u&&(v+=u);r&&(v+="&useragent="+r);t&&
(v+="&brid="+t.ID+"&brver="+t.version+"&bridua="+t.ID_UA);v+="&dup="+dv_GetParam(m,"dup");try{v+=dv_AppendIQPAParams(m)}catch(I){}(p=dv_GetParam(m,"turl"))&&(v+="&turl="+p);(p=dv_GetParam(m,"tagformat"))&&(v+="&tagformat="+p);v+=W();q=q?"&dvf=0":"";p=L("maple")?"&dvf=1":"";d=(window._dv_win.dv_config.verifyJSURL||c.protocol+"//"+(window._dv_win.dv_config.bsAddress||"rtb"+c.dvregion+".doubleverify.com")+"/verify.js")+"?jsCallback="+c.callbackName+"&jsTagObjCallback="+c.tagObjectCallbackName+"&num=6"+
v+"&srcurlD="+d.depth+"&ssl="+c.ssl+q+p+"&refD="+x+c.tagIntegrityFlag+c.tagHasPassbackFlag+"&htmlmsging="+(l?"1":"0");(l=dv_GetDynamicParams(m,"dvp").join("&"))&&(d+="&"+l);(l=dv_GetDynamicParams(m,"dvpx").join("&"))&&(d+="&"+l);if(!1===k||h)d=d+("&dvp_isBodyExistOnLoad="+(k?"1":"0"))+("&dvp_isOnHead="+(h?"1":"0"));e="srcurl="+encodeURIComponent(e);(k=X())&&(e+="&ancChain="+encodeURIComponent(k));k=4E3;/MSIE (\d+\.\d+);/.test(navigator.userAgent)&&7>=new Number(RegExp.$1)&&(k=2E3);if(h=dv_GetParam(m,
"referrer"))h="&referrer="+h,d.length+h.length<=k&&(d+=h);(h=dv_GetParam(m,"prr"))&&(d+="&prr="+h);(h=dv_GetParam(m,"iframe"))&&(d+="&iframe="+h);(h=dv_GetParam(m,"gdpr"))&&(d+="&gdpr="+h);(h=dv_GetParam(m,"gdpr_consent"))&&(d+="&gdpr_consent="+h);g.length+y.length+d.length<=k&&(d+=y,e+=g);(g=Y())&&(d+="&m1="+g);(g=Z())&&0<g.f&&(d+="&bsig="+g.f,d+="&usig="+g.s);g=aa();0<g&&(d+="&hdsig="+g);navigator&&navigator.hardwareConcurrency&&(d+="&noc="+navigator.hardwareConcurrency);d+=ba();g=ca();d+="&vavbkt="+
g.vdcd;d+="&lvvn="+g.vdcv;""!=g.err&&(d+="&dvp_idcerr="+encodeURIComponent(g.err));"prerender"===window._dv_win.document.visibilityState&&(d+="&prndr=1");(m=dv_GetParam(m,"wrapperurl"))&&1E3>=m.length&&d.length+m.length<=k&&(d+="&wrapperurl="+m);d+="&"+b.getVersionParamName()+"="+b.getVersion();b="&eparams="+encodeURIComponent(D(e));d=d.length+b.length<=k?d+b:d+"&dvf=3";window.performance&&window.performance.mark&&window.performance.measure&&window.performance.getEntriesByName&&(window.performance.mark("dv_create_req_end"),
window.performance.measure("dv_creqte_req","dv_create_req_start","dv_create_req_end"),(b=window.performance.getEntriesByName("dv_creqte_req"))&&0<b.length&&(d+="&dvp_exetime="+b[0].duration.toFixed(2)));b=d.toLowerCase();for(var z in c)c.hasOwnProperty(z)&&-1===b.indexOf(z.toLowerCase()+"=")&&!z.match(/^tag[A-Z]/)&&-1<["number","string"].indexOf(typeof c[z])&&void 0!==c[z]&&-1===["protocol","callbackName","dvregion"].indexOf(z.toLowerCase())&&(d+=["&",z,"=",encodeURIComponent(c[z])].join(""));return{isSev1:!1,
url:d}}function W(){var b="";try{var c=window._dv_win.parent;b+="&chro="+(void 0===c.chrome?"0":"1");b+="&hist="+(c.history?c.history.length:"");b+="&winh="+c.innerHeight;b+="&winw="+c.innerWidth;b+="&wouh="+c.outerHeight;b+="&wouw="+c.outerWidth;c.screen&&(b+="&scah="+c.screen.availHeight,b+="&scaw="+c.screen.availWidth)}catch(e){}return b}function ca(){var b=[],c=function(b){d(b,null!=b.AZSD,9);d(b,b.location.hostname!=b.encodeURIComponent(b.location.hostname),10);d(b,null!=b.cascadeWindowInfo,
11);d(b,null!=b._rvz,32);d(b,null!=b.FO_DOMAIN,34);d(b,null!=b.va_subid,36);d(b,b._GPL&&b._GPL.baseCDN,40);d(b,e(b,"__twb__")&&e(b,"__twb_cb_"),43);d(b,null!=b.landingUrl&&null!=b.seList&&null!=b.parkingPPCTitleElements&&null!=b.allocation,45);d(b,e(b,"_rvz",function(b){return null!=b.publisher_subid}),46);d(b,null!=b.cacildsFunc&&null!=b.n_storesFromFs,47);d(b,b._pcg&&b._pcg.GN_UniqueId,54);d(b,e(b,"__ad_rns_")&&e(b,"_$_"),64);d(b,null!=b.APP_LABEL_NAME_FULL_UC,71);d(b,null!=b._priam_adblock,81);
d(b,b.supp_ads_host&&b.supp_ads_host_overridden,82);d(b,b.uti_xdmsg_manager&&b.uti_xdmsg_manager.cb,87);d(b,b.LogBundleData&&b.addIframe,91);d(b,b.xAdsXMLHelperId||b.xYKAffSubIdTag,95);d(b,b.__pmetag&&b.__pmetag.uid,98);d(b,b.CustomWLAdServer&&/(n\d{1,4}adserv)|(1ads|cccpmo|epommarket|epmads|adshost1)/.test(b.supp_ads_host_overridden),100)},e=function(b,c,d){for(var f in b)if(-1<f.indexOf(c)&&(!d||d(b[f])))return!0;return!1},d=function(c,d,e){d&&-1==b.indexOf(e)&&b.push((c==window.top?-1:1)*e)};try{return function(){for(var b=
window,d=0;10>d&&(c(b),b!=window.top);d++)try{b.parent.document&&(b=b.parent)}catch(l){break}}(),{vdcv:28,vdcd:b.join(","),err:void 0}}catch(f){return{vdcv:28,vdcd:"-999",err:f.message||"unknown"}}}function S(b){for(var c=0;10>c&&b!=window._dv_win.top;)c++,b=b.parent;return c}function T(b,c,e){try{var d;if(!(d=e.ID==$dvbs.Enums.BrowserId.IE)){if(b==window._dv_win.top)var f=$dvbs.Enums.TrafficScenario.OnPage;else{e=b;try{for(var n=0;window._dv_win.top!=e&&10>=n;){var l=e.parent;if(!l.document)break;
e=l;n++}}catch(V){}f=e==window._dv_win.top?$dvbs.Enums.TrafficScenario.SameDomain:$dvbs.Enums.TrafficScenario.CrossDomain}d=f!=$dvbs.Enums.TrafficScenario.CrossDomain}if(d)return null;b.performance&&b.performance.mark&&b.performance.mark("dv_str_html_start");if(c){var k=c.toString().match(/^(?:https?:\/\/)?[\w\-\.]+\/[a-zA-Z0-9]/gi);if(k&&0<k.length)return null}var h=b.document;if(h&&h.referrer){var q=h.referrer.replace(/\//g,"\\/").replace(/\./g,"\\."),p=new RegExp('(?:w{0,4}=")?'+q+"[^&\"; %,'\\$\\\\\\|]+",
"gi");c=/banner|adprefs|safeframe|sandbox|sf\.html/gi;d=/^\w{0,4}="/gi;var A=J(h,"script","src",p,c,d);if(!A){var r=h.referrer;f="";var t=h.getElementsByTagName("script");if(t)for(l=0;!f&&l<t.length;){var g=t[l].innerHTML;if(g&&-1!=g.indexOf(r)){var y=g.match(p);f=K(y,c,d)}l++}(A=f)||(A=J(h,"a","href",p,c,d))}h=htmlUrl=A;a:{if(b.performance&&b.performance.mark&&b.performance.measure&&b.performance.getEntriesByName){b.performance.mark("dv_str_html_end");b.performance.measure("dv_str_html","dv_str_html_start",
"dv_str_html_end");var w=b.performance.getEntriesByName("dv_str_html");if(w&&0<w.length){var u=w[0].duration.toFixed(2);break a}}u=null}return{url:h,depth:-1,duration:u}}}catch(V){}return null}function K(b,c,e){var d="";if(b&&0<b.length)for(var f=0;f<b.length;f++){var n=b[f];n.length>d.length&&null==n.match(c)&&0!=n.indexOf('src="')&&0!=n.indexOf('turl="')&&(d=n.replace(e,""))}return d}function J(b,c,e,d,f,n){b=b.querySelectorAll(c+"["+e+'*="'+b.referrer+'"]');var l="";if(b)for(c=0;!l&&c<b.length;)l=
b[c][e].match(d),l=K(l,f,n),c++;return l}function U(b){try{if(1>=b.depth)return{url:"",depth:""};var c=[];c.push({win:window._dv_win.top,depth:0});for(var e,d=1,f=0;0<d&&100>f;){try{if(f++,e=c.shift(),d--,0<e.win.location.toString().length&&e.win!=b)return 0==e.win.document.referrer.length||0==e.depth?{url:e.win.location,depth:e.depth}:{url:e.win.document.referrer,depth:e.depth-1}}catch(k){}var n=e.win.frames.length;for(var l=0;l<n;l++)c.push({win:e.win.frames[l],depth:e.depth+1}),d++}return{url:"",
depth:""}}catch(k){return{url:"",depth:""}}}function X(){var b=window._dv_win[D("=@42E:@?")][D("2?46DE@C~C:8:?D")];if(b&&0<b.length){var c=[];c[0]=window._dv_win.location.protocol+"//"+window._dv_win.location.hostname;for(var e=0;e<b.length;e++)c[e+1]=b[e];return c.reverse().join(",")}return null}function D(b){new String;var c=new String,e;for(e=0;e<b.length;e++){var d=b.charAt(e);var f="!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~".indexOf(d);0<=
f&&(d="!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~".charAt((f+47)%94));c+=d}return c}function B(){return Math.floor(1E12*(Math.random()+""))}function R(b){for(var c=[{id:4,brRegex:"OPR|Opera",verRegex:"(OPR/|Version/)"},{id:1,brRegex:"MSIE|Trident/7.*rv:11|rv:11.*Trident/7|Edge/|Edg/",verRegex:"(MSIE |rv:| Edge/|Edg/)"},{id:2,brRegex:"Firefox",verRegex:"Firefox/"},{id:0,brRegex:"Mozilla.*Android.*AppleWebKit(?!.*Chrome.*)|Linux.*Android.*AppleWebKit.* Version/.*Chrome",
verRegex:null},{id:0,brRegex:"AOL/.*AOLBuild/|AOLBuild/.*AOL/|Puffin|Maxthon|Valve|Silk|PLAYSTATION|PlayStation|Nintendo|wOSBrowser",verRegex:null},{id:3,brRegex:"Chrome",verRegex:"Chrome/"},{id:5,brRegex:"Safari|(OS |OS X )[0-9].*AppleWebKit",verRegex:"Version/"}],e=0,d="",f=0;f<c.length;f++)if(null!=b.match(new RegExp(c[f].brRegex))){e=c[f].id;if(null==c[f].verRegex)break;b=b.match(new RegExp(c[f].verRegex+"[0-9]*"));null!=b&&(d=b[0].match(new RegExp(c[f].verRegex)),d=b[0].replace(d[0],""));break}c=
da();4==e&&(c=e);return{ID:c,version:c===e?d:"",ID_UA:e}}function da(){try{if(null!=window._phantom||null!=window.callPhantom)return 99;if(document.documentElement.hasAttribute&&document.documentElement.hasAttribute("webdriver")||null!=window.domAutomation||null!=window.domAutomationController||null!=window._WEBDRIVER_ELEM_CACHE)return 98;if(void 0!=window.opera&&void 0!=window.history.navigationMode||void 0!=window.opr&&void 0!=window.opr.addons&&"function"==typeof window.opr.addons.installExtension)return 4;
if(void 0!=document.uniqueID&&"string"==typeof document.uniqueID&&(void 0!=document.documentMode&&0<=document.documentMode||void 0!=document.all&&"object"==typeof document.all||void 0!=window.ActiveXObject&&"function"==typeof window.ActiveXObject)||window.document&&window.document.updateSettings&&"function"==typeof window.document.updateSettings||Object.values&&navigator&&Object.values(navigator.plugins).some(function(b){return-1!=b.name.indexOf("Edge PDF")}))return 1;if(void 0!=window.chrome&&"function"==
typeof window.chrome.csi&&"function"==typeof window.chrome.loadTimes&&void 0!=document.webkitHidden&&(1==document.webkitHidden||0==document.webkitHidden))return 3;if(void 0!=window.mozInnerScreenY&&"number"==typeof window.mozInnerScreenY&&void 0!=window.mozPaintCount&&0<=window.mozPaintCount&&void 0!=window.InstallTrigger&&void 0!=window.InstallTrigger.install)return 2;var b=!1;try{var c=document.createElement("p");c.innerText=".";c.style="text-shadow: rgb(99, 116, 171) 20px -12px 2px";b=void 0!=
c.style.textShadow}catch(e){}return(0<Object.prototype.toString.call(window.HTMLElement).indexOf("Constructor")||window.webkitAudioPannerNode&&window.webkitConvertPointFromNodeToPage)&&b&&void 0!=window.innerWidth&&void 0!=window.innerHeight?5:0}catch(e){return 0}}function Y(){try{var b=0,c=function(c,d){d&&32>c&&(b=(b|1<<c)>>>0)},e=function(b,c){return function(){return b.apply(c,arguments)}},d="svg"===document.documentElement.nodeName.toLowerCase(),f=function(){return"function"!==typeof document.createElement?
document.createElement(arguments[0]):d?document.createElementNS.call(document,"http://www.w3.org/2000/svg",arguments[0]):document.createElement.apply(document,arguments)},n=["Moz","O","ms","Webkit"],l=["moz","o","ms","webkit"],k={style:f("modernizr").style},h=function(b,c){function d(){g&&(delete k.style,delete k.modElem)}var e;for(e=["modernizr","tspan","samp"];!k.style&&e.length;){var g=!0;k.modElem=f(e.shift());k.style=k.modElem.style}var p=b.length;for(e=0;e<p;e++){var h=b[e];~(""+h).indexOf("-")&&
(h=cssToDOM(h));if(void 0!==k.style[h])return d(),"pfx"==c?h:!0}d();return!1},q=function(b,c,d){var f=b.charAt(0).toUpperCase()+b.slice(1),g=(b+" "+n.join(f+" ")+f).split(" ");if("string"===typeof c||"undefined"===typeof c)return h(g,c);g=(b+" "+l.join(f+" ")+f).split(" ");for(var k in g)if(g[k]in c){if(!1===d)return g[k];b=c[g[k]];return"function"===typeof b?e(b,d||c):b}return!1};c(0,!0);c(1,q("requestFileSystem",window));c(2,window.CSS?"function"==typeof window.CSS.escape:!1);c(3,q("shapeOutside",
"content-box",!0));return b}catch(p){return 0}}function E(){var b=window,c=0;try{for(;b.parent&&b!=b.parent&&b.parent.document&&!(b=b.parent,10<c++););}catch(e){}return b}function Z(){try{var b=E(),c=0,e=0,d=function(b,d,f){f&&(c+=Math.pow(2,b),e+=Math.pow(2,d))},f=b.document;d(14,0,b.playerInstance&&f.querySelector('script[src*="ads-player.com"]'));d(14,1,(b.CustomWLAdServer||b.DbcbdConfig)&&(a=f.querySelector('p[class="footerCopyright"]'),a&&a.textContent.match(/ MangaLife 2016/)));d(15,2,b.zpz&&
b.zpz.createPlayer);d(15,3,b.vdApp&&b.vdApp.createPlayer);d(15,4,f.querySelector('body>div[class="z-z-z"]'));d(16,5,b.xy_checksum&&b.place_player&&(b.logjwonready&&b.logContentPauseRequested||b.jwplayer));d(17,6,b==b.top&&""==f.title?(a=f.querySelector('body>object[id="player"]'),a&&a.data&&1<a.data.indexOf("jwplayer")&&"visibility: visible;"==a.getAttribute("style")):!1);d(17,7,f.querySelector('script[src*="sitewebvideo.com"]'));d(17,8,b.InitPage&&b.cef&&b.InitAd);d(17,9,b==b.top&&""==f.title?(a=
f.querySelector("body>#player"),null!=a&&null!=(null!=a.querySelector('div[id*="opti-ad"]')||a.querySelector('iframe[src="about:blank"]'))):!1);d(17,10,b==b.top&&""==f.title&&b.InitAdPlayer?(a=f.querySelector('body>div[id="kt_player"]'),null!=a&&null!=a.querySelector('div[class="flash-blocker"]')):!1);d(17,11,null!=b.clickplayer&&null!=b.checkRdy2);d(19,12,b.instance&&b.inject&&f.querySelector('path[id="cp-search-0"]'));d(20,13,function(){try{if(b.top==b&&0<b.document.getElementsByClassName("asu").length)for(var c=
b.document.styleSheets,d=0;d<c.length;d++)for(var e=b.document.styleSheets[d].cssRules,f=0;f<e.length;f++)if("div.kk"==e[f].selectorText||"div.asu"==e[f].selectorText)return!0}catch(g){}}());a:{try{var n=null!=f.querySelector('div[id="kt_player"][hiegth]');break a}catch(p){}n=void 0}d(21,14,n);a:{try{var l=b.top==b&&null!=b.document.querySelector('div[id="asu"][class="kk"]');break a}catch(p){}l=void 0}d(22,15,l);a:{try{var k=f.querySelector('object[data*="/VPAIDFlash.swf"]')&&f.querySelector('object[id*="vpaid_video_flash_tester_el"]')&&
f.querySelector('div[id*="desktopSubModal"]');break a}catch(p){}k=void 0}d(25,16,k);var h=navigator.userAgent;if(h&&-1<h.indexOf("Android")&&-1<h.indexOf(" wv)")&&b==window.top){var q=f.querySelector('img[src*="dealsneartome.com"]')||(b.__cads__?!0:!1)||0<f.querySelectorAll('img[src*="/tracker?tag="]').length;d(28,17,q||!1)}return{f:c,s:e}}catch(p){return null}}function aa(){try{var b=E(),c=0,e=b.document;b==window.top&&""==e.title&&!e.querySelector("meta[charset]")&&e.querySelector('div[style*="background-image: url("]')&&
(e.querySelector('script[src*="j.pubcdn.net"]')||e.querySelector('span[class="ad-close"]'))&&(c+=Math.pow(2,6));return c}catch(d){return null}}function ba(){try{var b="&fcifrms="+window.top.length;window.history&&(b+="&brh="+window.history.length);var c=E(),e=c.document;if(c==window.top){b+="&fwc="+((c.FB?1:0)+(c.twttr?2:0)+(c.outbrain?4:0)+(c._taboola?8:0));try{e.cookie&&(b+="&fcl="+e.cookie.length)}catch(d){}c.performance&&c.performance.timing&&0<c.performance.timing.domainLookupStart&&0<c.performance.timing.domainLookupEnd&&
(b+="&flt="+(c.performance.timing.domainLookupEnd-c.performance.timing.domainLookupStart));e.querySelectorAll&&(b+="&fec="+e.querySelectorAll("*").length)}return b}catch(d){return""}}var x=this,u=function(){function b(b,c){var e=[];c&&n.forEach(function(b){var d=dv_GetParam(c,b);""!==d&&null!==d&&e.push(["dvp_"+b,d].join("="))});var h=window&&window._dv_win||{};h=h.dv_config=h.dv_config||{};h=dv_getDVBSErrAddress(h);var l=[f,d].join("="),g=["dvp_cert",k[b]].join("=");b=["dvp_jsErrMsg",b].join("=");
h+=["/verify.js?ctx=818052&cmp=1619415&dvp_isLostImp=1&ssl=1",l,g,b,e.join("&")].join("&");(new Image(1,1)).src="https://"+h}function c(c,d){var e=window._dv_win.dv_config.bs_renderingMethod||function(b){document.write(b)};d="AdRenderedUponVerifyFailure__"+(d||"");if(x&&x.tagParamsObj&&x.tagParamsObj.tagAdtag)try{e(x.tagParamsObj.tagAdtag)}catch(r){d+="__RenderingMethodFailed"}else x?x.tagParamsObj?x.tagParamsObj.tagAdtag||(d+="__HandlerTagParamsObjTagAdtag__Undefined"):d+="__HandlerTagParamsObj__Undefined":
d+="__Handler__Undefined";b(d,c)}var e=!1,d,f,n=["ctx","cmp","plc","sid"],l=[x.constructor&&x.constructor.name||"UKDV","__",B()].join(""),k={VerifyLoadJSONPCallbackFailed:1,VerifyFailedToLoad:2},h={onResponse:function(d){e||(b("VerifyCallbackFailed",d),c(d,"VCF"))},onError:function(d){b("VerifyFailedToLoad",d);c(d,"VFTL")}};h.reportError=b;h.isJSONPCalled=e;window._dv_win[l]={globalScopeVerifyErrorHandler:h};return{setVersionData:function(b,c){f=b;d=c},setIsJSONPCalled:function(b){e=b},getIsJSONPCalled:function(){return e},
onLoad:dv_onResponse,onError:dv_onError,uniqueKey:l}}();this.createRequest=function(){window.performance&&window.performance.mark&&window.performance.mark("dv_create_req_start");var b=!1,c=window._dv_win,e=0,d=!1,f;try{for(f=0;10>=f;f++)if(null!=c.parent&&c.parent!=c)if(0<c.parent.location.toString().length)c=c.parent,e++,b=!0;else{b=!1;break}else{0==f&&(b=!0);break}}catch(y){b=!1}a:{try{var n=c.$sf;break a}catch(y){}n=void 0}var l=(f=c.location&&c.location.ancestorOrigins)&&f[f.length-1];if(0==c.document.referrer.length)b=
c.location;else if(b)b=c.location;else{b=c.document.referrer;a:{try{var k=c.$sf&&c.$sf.ext&&c.$sf.ext.hostURL&&c.$sf.ext.hostURL();break a}catch(y){}k=void 0}if(k&&(!f||0==k.indexOf(l))){b=k;var h=!0}d=!0}if(!window._dv_win.dvbsScriptsInternal||!window._dv_win.dvbsProcessed||0==window._dv_win.dvbsScriptsInternal.length)return{isSev1:!1,url:null};f=window._dv_win.dv_config&&window._dv_win.dv_config.isUT?window._dv_win.dvbsScriptsInternal[window._dv_win.dvbsScriptsInternal.length-1]:window._dv_win.dvbsScriptsInternal.pop();
k=f.script;this.dv_script_obj=f;this.dv_script=k;window._dv_win.dvbsProcessed.push(f);window._dv_win._dvScripts.push(k);var q=k.src;this.dvOther=0;this.dvStep=1;var p=window._dv_win.dv_config?window._dv_win.dv_config.bst2tid?window._dv_win.dv_config.bst2tid:window._dv_win.dv_config.dv_GetRnd?window._dv_win.dv_config.dv_GetRnd():B():B();f=window.parent.postMessage&&window.JSON;l=!0;var u=!1;if("0"==dv_GetParam(q,"t2te")||window._dv_win.dv_config&&1==window._dv_win.dv_config.supressT2T)u=!0;if(f&&0==
u)try{var r=F(window._dv_win.dv_config.bst2turl||"https://cdn3.doubleverify.com/bst2tv3.html","bst2t_"+p);l=C(r)}catch(y){}r={};try{u=/[\?&]([^&]*)=([^&#]*)/gi;for(var t=u.exec(q);null!=t;)"eparams"!==t[1]&&(r[t[1]]=t[2]),t=u.exec(q);var g=r}catch(y){g=r}this.tagParamsObj=g;g.perf=this.perf;g.uid=p;g.script=this.dv_script;g.callbackName="__verify_callback_"+g.uid;g.tagObjectCallbackName="__tagObject_callback_"+g.uid;g.tagAdtag=null;g.tagPassback=null;g.tagIntegrityFlag="";g.tagHasPassbackFlag="";
0==(null!=g.tagformat&&"2"==g.tagformat)&&(t=Q(g.script),g.tagAdtag=t.tagAdTag,g.tagPassback=t.tagPassback,t.isBadImp?g.tagIntegrityFlag="&isbadimp=1":t.hasPassback&&(g.tagHasPassbackFlag="&tagpb=1"));(t=(/iPhone|iPad|iPod|\(Apple TV|iOS|Coremedia|CFNetwork\/.*Darwin/i.test(navigator.userAgent)||navigator.vendor&&"apple, inc."===navigator.vendor.toLowerCase())&&!window.MSStream)?r="https:":(r=g.script.src,q="http:",p=window._dv_win.location.toString().match("^http(?:s)?"),"https"!=r.match("^https")||
"https"!=p&&"http"==p||(q="https:"),r=q);g.protocol=r;g.ssl="0";"https:"===g.protocol&&(g.ssl="1");r=g;(q=window._dv_win.dvRecoveryObj)?("2"!=r.tagformat&&(q=q[r.ctx]?q[r.ctx].RecoveryTagID:q._fallback_?q._fallback_.RecoveryTagID:1,1===q&&r.tagAdtag?document.write(r.tagAdtag):2===q&&r.tagPassback&&document.write(r.tagPassback)),r=!0):r=!1;if(r)return{isSev1:!0};this.dvStep=2;P(g,t);k=k&&k.parentElement&&k.parentElement.tagName&&"HEAD"===k.parentElement.tagName;this.dvStep=3;return H(this,g,b,c,e,
d,f,l,k,t,h,n)};this.sendRequest=function(b){var c=dv_GetParam(b,"tagformat");if(u)try{u.setVersionData(this.getVersionParamName(),this.getVersion()),c&&"2"==c?$dvbs.domUtilities.addScriptResource(b,document.body,u.onLoad,u.onError,u.uniqueKey):dv_sendScriptRequest(b,u.onLoad,u.onError,u.uniqueKey)}catch(p){c&&"2"==c?$dvbs.domUtilities.addScriptResource(b,document.body):dv_sendScriptRequest(b)}else c&&"2"==c?$dvbs.domUtilities.addScriptResource(b,document.body):dv_sendScriptRequest(b);try{if("1"!=
dv_GetParam(b,"foie")){var e=N(this.dv_script_obj&&this.dv_script_obj.injScripts),d=F("about:blank");d.id=d.name;var f=d.id.replace("iframe_","");d&&"function"===typeof d.setAttribute&&d.setAttribute("data-dv-frm",f);C(d,this.dv_script);if(this.dv_script){var n=this.dv_script;a:{b=null;try{if(b=d.contentWindow){var l=b;break a}}catch(p){}try{if(b=window._dv_win.frames&&window._dv_win.frames[d.name]){l=b;break a}}catch(p){}l=null}n.dvFrmWin=l}a:{var k;if(d&&(k=d.contentDocument||d.contentWindow&&d.contentWindow.document)){var h=
k;break a}h=(k=window._dv_win.frames&&window._dv_win.frames[d.name]&&window._dv_win.frames[d.name].document)?k:null}if(h)h.open(),h.write(e);else{try{document.domain=document.domain}catch(p){}var q=encodeURIComponent(e.replace(/'/g,"\\'").replace(/\n|\r\n|\r/g,""));d.src='javascript: (function(){document.open();document.domain="'+window.document.domain+"\";document.write('"+q+"');})()"}}}catch(p){e=window._dv_win.dv_config=window._dv_win.dv_config||{},e=dv_getDVBSErrAddress(e),d=[this.getVersionParamName(),
this.getVersion()].join("="),e+=["/verify.js?ctx=818052&cmp=1619415&dvp_cert=3&num=6",d].join("&"),dv_SendErrorImp(e,[{dvp_jsErrMsg:"DvFrame: "+encodeURIComponent(p)}])}return!0};this.isApplicable=function(){return!0};this.onFailure=function(){};window.debugScript&&(window.CreateUrl=H);this.getVersionParamName=function(){return"ver"};this.getVersion=function(){return"147"}};


function dvbs_src_main(dvbs_baseHandlerIns, dvbs_handlersDefs) {

    this.bs_baseHandlerIns = dvbs_baseHandlerIns;
    this.bs_handlersDefs = dvbs_handlersDefs;

    this.exec = function () {
        try {
            window._dv_win = (window._dv_win || window);
            window._dv_win.$dvbs = (window._dv_win.$dvbs || new dvBsType());

            window._dv_win.dv_config = window._dv_win.dv_config || {};
            window._dv_win.dv_config.bsErrAddress = window._dv_win.dv_config.bsAddress || 'rtb0.doubleverify.com';

            var errorsArr = (new dv_rolloutManager(this.bs_handlersDefs, this.bs_baseHandlerIns)).handle();
            if (errorsArr && errorsArr.length > 0)
                dv_SendErrorImp(window._dv_win.dv_config.bsErrAddress + '/verify.js?ctx=818052&cmp=1619415&num=6', errorsArr);
        }
        catch (e) {
            try {
                dv_SendErrorImp(window._dv_win.dv_config.bsErrAddress + '/verify.js?ctx=818052&cmp=1619415&num=6&dvp_isLostImp=1', {dvp_jsErrMsg: encodeURIComponent(e)});
            } catch (e) {
            }
        }
    };
};

try {
    window._dv_win = window._dv_win || window;
    var dv_baseHandlerIns = new dv_baseHandler();
	

    var dv_handlersDefs = [];
    (new dvbs_src_main(dv_baseHandlerIns, dv_handlersDefs)).exec();
} catch (e) { }