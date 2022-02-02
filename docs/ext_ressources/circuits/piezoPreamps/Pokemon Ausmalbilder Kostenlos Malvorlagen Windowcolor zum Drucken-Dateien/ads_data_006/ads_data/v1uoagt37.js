



/* ControlTag Loader for Crocs DMP EU 37956252-0cbd-4292-a913-17f97e0c4e53 */
(function(w, cs) {
  
  if (/Twitter for iPhone/.test(w.navigator.userAgent || '')) {
    return;
  }

  var debugging = /kxdebug/.test(w.location);
  var log = function() {
    
    debugging && w.console && w.console.log([].slice.call(arguments).join(' '));
  };

  var load = function(url, callback) {
    log('Loading script from:', url);
    var node = w.document.createElement('script');
    node.async = true;  
    node.src = url;

    
    node.onload = node.onreadystatechange = function () {
      var state = node.readyState;
      if (!callback.done && (!state || /loaded|complete/.test(state))) {
        log('Script loaded from:', url);
        callback.done = true;  
        callback();
      }
    };

    
    var sibling = w.document.getElementsByTagName('script')[0];
    sibling.parentNode.insertBefore(node, sibling);
  };

  var config = {"app":{"name":"krux-scala-config-webservice","version":"3.42.4","schema_version":3},"confid":"v1uoagt37","context_terms":[],"publisher":{"name":"Crocs DMP EU","active":true,"uuid":"37956252-0cbd-4292-a913-17f97e0c4e53","version_bucket":"stable","id":5708},"params":{"link_header_bidder":false,"site_level_supertag_config":"site","recommend":false,"control_tag_pixel_throttle":100,"fingerprint":false,"optout_button_optout_text":"Browser Opt Out","channel":"display","user_data_timing":"load","consent_active":true,"use_central_usermatch":true,"store_realtime_segments":false,"tag_source":false,"link_hb_start_event":"ready","optout_button_optin_text":"Browser Opt In","link_hb_timeout":2000,"link_hb_adserver_subordinate":true,"optimize_realtime_segments":false,"link_hb_adserver":"dfp","client_type":"marketer","target_fingerprint":false,"context_terms":true,"optout_button_id":"kx-optout-button","dfp_premium":true,"control_tag_namespace":"crocsdmpeu","support_content_security_policy":false},"prioritized_segments":[],"realtime_segments":[],"services":{"userdata":"//cdn.krxd.net/userdata/get","contentConnector":"https://connector.krxd.net/content_connector","stats":"//apiservices.krxd.net/stats","optout":"//cdn.krxd.net/userdata/optout/status","event":"//beacon.krxd.net/event.gif","set_optout":"https://consumer.krxd.net/consumer/optout","data":"//beacon.krxd.net/data.gif","link_hb_stats":"//beacon.krxd.net/link_bidder_stats.gif","userData":"//cdn.krxd.net/userdata/get","link_hb_mas":"https://link.krxd.net/hb","config":"//cdn.krxd.net/controltag/{{ confid }}.js","social":"//beacon.krxd.net/social.gif","addSegment":"//cdn.krxd.net/userdata/add","pixel":"//beacon.krxd.net/pixel.gif","um":"https://usermatch.krxd.net/um/v2","controltag":"//cdn.krxd.net/ctjs/controltag.js.{hash}","loopback":"https://consumer.krxd.net/consumer/tmp_cookie","remove":"https://consumer.krxd.net/consumer/remove/37956252-0cbd-4292-a913-17f97e0c4e53","click":"https://apiservices.krxd.net/click_tracker/track","stats_export":"//beacon.krxd.net/controltag_stats.gif","userdataApi":"//cdn.krxd.net/userdata/v1/segments/get","cookie":"//beacon.krxd.net/cookie2json","proxy":"//cdn.krxd.net/partnerjs/xdi","consent_get":"https://consumer.krxd.net/consent/get/37956252-0cbd-4292-a913-17f97e0c4e53","consent_set":"https://consumer.krxd.net/consent/set/37956252-0cbd-4292-a913-17f97e0c4e53","is_optout":"https://beacon.krxd.net/optout_check","impression":"//beacon.krxd.net/ad_impression.gif","transaction":"//beacon.krxd.net/transaction.gif","log":"//jslog.krxd.net/jslog.gif","portability":"https://consumer.krxd.net/consumer/portability/37956252-0cbd-4292-a913-17f97e0c4e53","set_optin":"https://consumer.krxd.net/consumer/optin","usermatch":"//beacon.krxd.net/usermatch.gif"},"experiments":[],"site":{"name":"DE Display","cap":2,"id":1691562,"organization_id":5708,"uid":"v1uoagt37"},"tags":[{"id":46671,"name":"Standard - All Sites","content":"<script>\n(function() {\n    /* Standard but configurable DTC */\n    var comDomain, domain, level, libUtil, pathLevel, prefix, toSet;\n    libUtil = Krux('require:util.library-tag');\n    toSet = {};\n    level = 1;\n    while (level <= Number('3')) {\n        pathLevel = Krux('scrape.url_path', level);\n        if (pathLevel) {\n            if ('false' === 'true') {\n                pathLevel = (\"\" + pathLevel).replace(\n                    /\\.(?:html?|php[0-9]?|aspx?|cfg|py)$/i, '');\n            }\n            toSet[\"page_attr_url_path_\" + level] = pathLevel;\n        }\n        level++;\n    }\n    toSet['page_attr_meta_keywords'] = Krux('scrape.meta_name', 'keywords');\n    prefix = libUtil.resolvePrefix('none', 'undefined',\n        'undefined');\n    toSet = Krux('prefix:attr', toSet, prefix);\n    domain = Krux('get', 'domain');\n    comDomain = domain.match(/\\.([^\\.]+\\.com)$/);\n    if (comDomain) {\n        domain = comDomain[1];\n    }\n    toSet['page_attr_domain'] = domain;\n    Krux('set', toSet);\n}).call();\n</script>","target":null,"target_action":"append","timing":"onready","method":"document","priority":null,"template_replacement":true,"internal":true,"criteria":[],"collects_data":true},{"id":46667,"name":"DTC Whitelist - All Sites","content":"<script>\n(function() {\n    /* Selective Attribute DataLayer Library Tag */\n    var _, allAttr, allowedList, attr, attributes, dataLayerIngester, dataObj,\n        isAllowed, keepCase, libUtil, omitKeys, pageAttr, prefix, toSet, trim,\n        userAttr, util, value,\n        hasProp = {}.hasOwnProperty;\n    _ = Krux('require:underscore');\n    util = Krux('require:util');\n    libUtil = Krux('require:util.library-tag');\n    dataLayerIngester = Krux('require:scrape').ingestDataLayer;\n\n    /* Safe copy of dataLayer object */\n    dataObj = Krux('scrape.javascript', 'app');\n\n    /* String trimming helper function */\n    trim = function(attr) {\n        return (\"\" + attr).replace(/^\\s+|\\s+$/g, '');\n    };\n\n    /* Attribute configs */\n    pageAttr = _.map('productID,name,categoryID,categoryName'.split(','), trim);\n    userAttr = _.map('userID,email_sha256'.split(','), trim);\n\n    /* Create a array of attributes striping any empty strings */\n    allAttr = _.without(pageAttr.concat(userAttr), '');\n\n    /* Configuration settings */\n    keepCase = 'false' === 'true';\n    omitKeys = 'undefined'.split(',');\n\n    /* Resolve Prefix */\n    prefix = libUtil.resolvePrefix('none', 'undefined',\n        'undefined');\n\n    /* Function to varify if attribute should be used */\n    isAllowed = function(value, whitelist) {\n        var i, len, str, x;\n        str = \"\" + value;\n        if (!((value != null) && str.length > 0)) {\n            return false;\n        }\n        for (i = 0, len = whitelist.length; i < len; i++) {\n            x = whitelist[i];\n            if (value.match(x) != null) {\n                return true;\n            }\n        }\n        return false;\n    };\n\n    /* Get a full list of attributes usting the dataLayer tool */\n    attributes = dataLayerIngester(dataObj, {\n        omitKeys: libUtil.removeFalsyStrings(omitKeys.concat(libUtil.EXCLUDE_KEYS_CONFIG)),\n        omitValues: libUtil.EXCLUDE_VALUES_CONFIG,\n        caseSensitive: keepCase,\n        useFullPath: 'false' === 'true',\n        useLastValue: 'false' === 'true',\n        customDelimited: [/./],\n        altDelimiter: ',',\n        userKeys: _.map(userAttr, function(exp) {\n            return new RegExp(\"(^|\\\\.)\" + exp + \"$\");\n        }),\n        optimizeNames: true\n    });\n\n    /* Only set Attributes in the allowed list */\n    allowedList = _.map(allAttr, function(name) {\n        return new RegExp(\"(_attr_|_attr_\" + prefix + \"|\\\\.)\" + (keepCase ?\n            name : libUtil.normalizeAttrName(name, {\n                removeDot: false\n            })) + \"$\");\n    });\n    toSet = {};\n    for (attr in attributes) {\n        if (!hasProp.call(attributes, attr)) continue;\n        value = attributes[attr];\n        if (isAllowed(attr, allowedList)) {\n            toSet[attr] = value;\n        }\n    }\n    toSet = Krux('prefix:attr', toSet, prefix);\n    Krux('set', toSet);\n}).call();\n</script>","target":null,"target_action":"append","timing":"onload","method":"document","priority":null,"template_replacement":true,"internal":true,"criteria":[],"collects_data":true},{"id":46668,"name":"UTM - All Sites","content":"<script>\n(function(){\n\n\tvar params = Krux('require:util').urlParams();\n\t\n\tKrux ('set', { \n\t'page_attr_utm_source': params.utm_source,\n\t'page_attr_utm_medium': params.utm_medium,\n\t'page_attr_utm_campaign': params.utm_campaign,\n\t'page_attr_utm_content': params.utm_content,\n\t'page_attr_utm_term': params.utm_term \n\t});\n\t\n})();\n</script>","target":null,"target_action":"append","timing":"onload","method":"document","priority":null,"template_replacement":true,"internal":true,"criteria":[],"collects_data":true}],"usermatch_tags":[{"id":6,"name":"Google User Match","content":"<script>\r\n(function() {\r\n\r\nvar kuid = Krux('get', 'user');\r\n  if(kuid){\r\n  // original google user match tag. will be deprecated june 1, 2020\r\n  new Image().src = 'https://usermatch.krxd.net/um/v2?partner=google';\r\n\r\n  // new google user match where they host the match table. The KUID needs to be base64 encoded, but the ids sent will be regular kuids\r\n  var baseEncodedKuid = btoa(kuid).replace(/=$/, '');\r\n  new Image().src = 'https://cm.g.doubleclick.net/pixel?google_nid=krux_digital&google_cm&google_hm='+baseEncodedKuid;\r\n  }\r\n\r\n})();\r\n</script>","target":null,"target_action":"append","timing":"onload","method":"document","priority":1,"template_replacement":false,"internal":true,"criteria":[],"collects_data":true},{"id":15,"name":"TheTradeDesk User Match","content":"<script>\r\n(function(){\r\nvar kuid = Krux('get', 'user');\r\nvar consent = Krux('iab:urlParams','gdpr_consent','gdpr',true);\r\nif (kuid) {\r\nif(!consent){\r\nconsent = \"&gdpr=0\";\r\n}\r\nvar p = location.protocol == 'https:' ? 'https:' : 'http:';\r\nvar purl = '//match.adsrvr.org/track/cmf/generic?ttd_pid=krux&ttd_tpi=1&ttd_puid=';\r\n(new Image()).src  = p + purl +kuid+ consent; \r\n}\r\n})();\r\n</script>","target":null,"target_action":"append","timing":"onload","method":"document","priority":1,"template_replacement":false,"internal":true,"criteria":[],"collects_data":true},{"id":41,"name":"Adobe user match","content":"<script>\r\n(function(){\r\n   var kuid = Krux('get', 'user');\r\n   if (kuid) {\r\n\tvar prefix = window.location.protocol == 'https:' ? 'https:' : 'http:';\r\n\tvar url = prefix + '//dpm.demdex.net/ibs:dpid=66757&&dpuuid=' + kuid + '&redir=' + encodeURIComponent(prefix)+'%2F%2Fbeacon.krxd.net%2Fusermatch.gif%3Fpartner%3Dadobe%26partner_uid%3D${DD_UUID}';\r\n\tnew Image().src = url;\r\n  }\r\n\r\n})();\r\n</script>","target":null,"target_action":"append","timing":"onload","method":"document","priority":1,"template_replacement":false,"internal":true,"criteria":[],"collects_data":true},{"id":71,"name":"Xandr Connect","content":"<script>\r\n(function(){\r\n        var kuid = Krux('get', 'user');\r\n        var consent = Krux('iab:urlParams');\r\n        if (kuid) {\r\n            if(!consent){\r\n                consent = \"\";\r\n            }\r\n            var prefix = location.protocol == 'https:' ? \"https:\" : \"http:\";\r\n            var kurl = prefix + '//beacon.krxd.net/usermatch.gif?adnxs_uid=$UID';\r\n            var appnexus_url = '//ib.adnxs.com/getuid?' + kurl + consent;\r\n            (new Image()).src=appnexus_url;\r\n        }\r\n})();\r\n</script>\r\n\r\n<!-- Krux Config:\r\n\r\n-->","target":null,"target_action":"append","timing":"onload","method":"document","priority":1,"template_replacement":false,"internal":true,"criteria":[],"collects_data":true},{"id":99,"name":"Amazon User Match Pixel","content":"<script>\r\n(function(){\r\n        var kuid = Krux('get', 'user');\r\n        var consent = Krux('iab:urlParams','gdpr_consent','gdpr',true);\r\n        if (kuid) {\r\n            var url = 'https://usermatch.krxd.net/um/v2?partner=amazon_eu';\r\n            if(!consent){\r\n                consent = \"&gdpr=0\";\r\n                url = 'https://usermatch.krxd.net/um/v2?partner=amazon_na';\r\n            }\r\n            url =  url + consent;\r\n            (new Image()).src = url;\r\n        }\r\n    })();\r\n</script>","target":null,"target_action":"append","timing":"onload","method":"document","priority":1,"template_replacement":false,"internal":true,"criteria":[],"collects_data":true},{"id":28,"name":"Dstillery User Match","content":"<script>\n(function(){\n        var kuid = Krux('get', 'user');\n        var prefix = window.location.protocol == 'https:' ? 'https:' : 'http:';\n        if (kuid) {\n        var url = prefix + '//idpix.media6degrees.com/orbserv/hbpix?pixId=30036&pcv=27';\n        (new Image()).src = url;\n        }\n})();\n</script>","target":null,"target_action":"append","timing":"onload","method":"document","priority":2,"template_replacement":false,"internal":true,"criteria":[],"collects_data":true},{"id":79,"name":"Quantcast S2S User Match","content":"<script>\r\n(function(){\r\n        var kuid = Krux('get', 'user');\r\n        if (kuid) {\r\n            (new Image()).src ='https://usermatch.krxd.net/um/v2?partner=quantcast';\r\n        }\r\n    })();\r\n</script>","target":null,"target_action":"append","timing":"onload","method":"document","priority":3,"template_replacement":false,"internal":true,"criteria":[],"collects_data":true}],"link":{"adslots":{},"bidders":{}}};
  
  for (var i = 0, tags = config.tags, len = tags.length, tag; (tag = tags[i]); ++i) {
    if (String(tag.id) in cs) {
      tag.content = cs[tag.id];
    }
  }

  
  var esiGeo = String(function(){/*
   <esi:include src="/geoip_esi"/>
  */}).replace(/^.*\/\*[^{]+|[^}]+\*\/.*$/g, '');

  if (esiGeo) {
    log('Got a request for:', esiGeo, 'adding geo to config.');
    try {
      config.geo = w.JSON.parse(esiGeo);
    } catch (__) {
      
      log('Unable to parse geo from:', config.geo);
      config.geo = {};
    }
  }



  var proxy = (window.Krux && window.Krux.q && window.Krux.q[0] && window.Krux.q[0][0] === 'proxy');

  if (!proxy || true) {
    

  load('//cdn.krxd.net/ctjs/controltag.js.a1705c5ac5f06cf0c202ff70908fc042', function() {
    log('Loaded stable controltag resource');
    Krux('config', config);
  });

  }

})(window, (function() {
  var obj = {};
  
  return obj;
})());
