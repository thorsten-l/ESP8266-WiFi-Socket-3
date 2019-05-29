#include <App.hpp>

const char MAINTENANCE_HTML[] PROGMEM =
  "<!DOCTYPE html>\n"
  "<html>\n"
  "<head>\n"
  "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n"
  "<title>WIFI SOCKET II - SETUP</title>\n"
  "<link rel=\"stylesheet\" href=\"pure-min.css\">\n"
  "<link rel=\"stylesheet\" href=\"layout.css\">\n"
  "</head>\n"
  "<body>\n"
  "<div class=\"custom-menu-wrapper\">\n"
  "<div class=\"pure-menu custom-menu custom-menu-top\">\n"
  "<div id=\"brand-title\" class=\"pure-menu-heading custom-menu-brand\">WiFi Socket II - Setup</div><a href=\"#\" class=\"custom-menu-toggle\" id=\"toggle\"><s class=\"bar\"></s><s class=\"bar\"></s></a></div>\n"
  "<div class=\"pure-menu pure-menu-horizontal pure-menu-scrollable custom-menu custom-menu-bottom custom-menu-tucked\" id=\"tuckedMenu\">\n"
  "<div class=\"custom-menu-screen\"></div>\n"
  "<ul class=\"pure-menu-list\">\n"
  "<li class=\"pure-menu-item\"><a href=\"/\" class=\"pure-menu-link\">Home</a></li>\n"
  "<li class=\"pure-menu-item\"><a href=\"/setup.html\" class=\"pure-menu-link\">Setup</a></li>\n"
  "<li class=\"pure-menu-item\"><a href=\"/maintenance.html\" class=\"pure-menu-link\">Maintenance</a></li>"
  "<li class=\"pure-menu-item\"><a href=\"/info.html\" class=\"pure-menu-link\">Info</a></li>\n"
  "</ul>\n"
  "</div>\n"
  "</div>\n"
  "<div class=\"main\">\n"
  "<div class=\"content\">\n"

           "<div class='accordion'>Backup & Restore Configuration</div>\n"
            "<div class='panel sub-panel'>\n"

                "<form class='pure-form pure-form-aligned' method='GET' action='/config-backup' >\n"
                    "<fieldset>\n"
                        "<legend>Backup Configuration</legend>\n"
                        "<p><input class='pure-button pure-button-primary' type='submit' value='Download'></p>\n"
                    "</fieldset>\n"
                "</form>\n"

                "<form class='pure-form pure-form-aligned' method='POST' action='/config-restore' enctype='multipart/form-data'>\n"
                    "<fieldset>\n"
                        "<legend>Restore Configuration</legend>\n"
                        "<div>\n"
                            "<input class='inputfile' id='ucid34' type='file' name='upload'>\n"
                            "<label for='ucid34'><svg xmlns='http://www.w3.org/2000/svg' width='24' height='20' viewBox='0 0 24 20'><path d='M10 0l-5.2 4.9h3.3v5.1h3.8v-5.1h3.3l-5.2-4.9zm9.3 11.5l-3.2-2.1h-2l3.4 2.6h-3.5c-.1 0-.2.1-.2.1l-.8 2.3h-6l-.8-2.2c-.1-.1-.1-.2-.2-.2h-3.6l3.4-2.6h-2l-3.2 2.1c-.4.3-.7 1-.6 1.5l.6 3.1c.1.5.7.9 1.2.9h16.3c.6 0 1.1-.4 1.3-.9l.6-3.1c.1-.5-.2-1.2-.7-1.5z'/></svg> <span>Choose a <b><tt>config.json</tt></b> file&hellip;</span></label>\n"
                        "</div>\n"
                        "<p><input class='pure-button pure-button-primary' type='submit' value='Upload'></p>\n"
                    "</fieldset>\n"
                "</form>\n"
            "</div>\n"
            "</div>\n"

           "<div class='accordion'>Firmware</div>\n"
            "<div class='panel sub-panel'>\n"

                "<form class='pure-form pure-form-aligned' method='POST' action='/update-firmware' enctype='multipart/form-data'>\n"
                    "<fieldset>\n"
                        "<legend>Upload new firmware</legend>\n"
                        "<div>\n"
                            "<input class='inputfile' id='pgid34' type='file' name='update'>\n"
                            "<label for='pgid34'><svg xmlns='http://www.w3.org/2000/svg' width='24' height='20' viewBox='0 0 24 20'><path d='M10 0l-5.2 4.9h3.3v5.1h3.8v-5.1h3.3l-5.2-4.9zm9.3 11.5l-3.2-2.1h-2l3.4 2.6h-3.5c-.1 0-.2.1-.2.1l-.8 2.3h-6l-.8-2.2c-.1-.1-.1-.2-.2-.2h-3.6l3.4-2.6h-2l-3.2 2.1c-.4.3-.7 1-.6 1.5l.6 3.1c.1.5.7.9 1.2.9h16.3c.6 0 1.1-.4 1.3-.9l.6-3.1c.1-.5-.2-1.2-.7-1.5z'/></svg> <span>Choose a <b><tt>%pioenv_name%</tt></b> firmware file&hellip;</span></label>\n"
                        "</div>\n"
                        "<p><input class='pure-button pure-button-primary' type='submit' value='Upload'></p>\n"
                    "</fieldset>\n"
                "</form>\n"

                "<form class='pure-form pure-form-aligned' method='POST' action='/reset-firmware' >\n"
                    "<fieldset>\n"
                        "<legend>Firmware Reset</legend>\n"
                        "<div>\n"
                            "<div class='pure-control-group'><label for='pgid35'>Really reset to system defaults?</label><select id='pgid35' name='doreset'><option selected='' value='false'>No</option><option value='true'>Yes</option></select></div>\n"
                        "</div>\n"
                        "<p><input class='pure-button pure-button-primary' type='submit' value='Reset'></p>\n"
                    "</fieldset>\n"
                "</form>\n"

            "</div>\n"

  "<p>&nbsp;</p></div>\n"
  "</div>\n"
  "<script>\n"
  "(function(window, document) {"
  "document.getElementById('toggle').addEventListener('click', function(e) {"
  "document.getElementById('tuckedMenu').classList.toggle('custom-menu-tucked');"
  "document.getElementById('toggle').classList.toggle('x');"
  "});"
  "})(this, this.document);"
  "</script>\n"
      "<script type='text/javascript'>\n"
        "var i, acc = document.getElementsByClassName('accordion');"
        "for (i = 0; i < acc.length; i++) acc[i].addEventListener('click', function() {"
            "this.classList.toggle('active');"
            "var e = this.nextElementSibling;"
            "'block' === e.style.display ? e.style.display = 'none' : e.style.display = 'block'"
        "});"
        "var input = document.getElementById('pgid34'),"
            "label = input.nextElementSibling,"
            "labelVal = label.innerHTML;"
        "input.addEventListener('change', function(e) {"
            "var l = e.target.value.split('\\\\').pop();"
            "l ? label.querySelector('span').innerHTML = l : label.innerHTML = labelVal"
        "});"
        "var input2 = document.getElementById('ucid34'),"
            "label2 = input2.nextElementSibling,"
            "labelVal2 = label2.innerHTML;"
        "input2.addEventListener('change', function(e) {"
            "var l2 = e.target.value.split('\\\\').pop();"
            "l2 ? label2.querySelector('span').innerHTML = l2 : label2.innerHTML = labelVal2"
        "});"
    "</script>\n"
  "</body>\n"
  "</html>\n";