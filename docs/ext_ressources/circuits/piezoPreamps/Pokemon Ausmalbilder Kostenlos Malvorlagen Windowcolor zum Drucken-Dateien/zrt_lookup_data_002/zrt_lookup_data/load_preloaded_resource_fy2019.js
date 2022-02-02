(function(){/* 
 
 Copyright The Closure Library Authors. 
 SPDX-License-Identifier: Apache-2.0 
*/ 
'use strict';/* 
 
 SPDX-License-Identifier: Apache-2.0 
*/ 
let e=[];const f=()=>{const b=e;e=[];for(const c of b)try{c()}catch(a){}};function g(b=null){return b&&"26"===b.getAttribute("data-jc")?b:document.querySelector('[data-jc="26"]')};var k=document;var l=()=>{var b=k.head,c=b.querySelectorAll("link[data-reload-stylesheet][as=style][rel=preload]");for(var a=0;a<c.length;a++){const n=c[a];var d="link",h=document;d=String(d);"application/xhtml+xml"===h.contentType&&(d=d.toLowerCase());d=h.createElement(d);d.setAttribute("rel","stylesheet");d.setAttribute("href",n.getAttribute("href"));b.appendChild(d)}if(0<c.length&&!(.01<Math.random())){b=g(document.currentScript);b=`https://${"pagead2.googlesyndication.com"}/pagead/gen_204?id=jca&jc=${26}&version=${b&& 
b.getAttribute("data-jc-version")||"unknown"}&sample=${.01}`;c=window;if(a=c.navigator)a=c.navigator.userAgent,a=/Chrome/.test(a)&&!/Edge/.test(a)?!0:!1;a&&c.navigator.sendBeacon?c.navigator.sendBeacon(b):(c.google_image_requests||(c.google_image_requests=[]),a=c.document,a=void 0===a?document:a,a=a.createElement("img"),a.src=b,c.google_image_requests.push(a))}};var m=document;"complete"===m.readyState||"interactive"===m.readyState?(e.push(l),1==e.length&&(window.Promise?Promise.resolve().then(f):window.setImmediate?setImmediate(f):setTimeout(f,0))):m.addEventListener("DOMContentLoaded",l);}).call(this);
