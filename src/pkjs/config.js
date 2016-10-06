module.exports = [
  {
    "type": "heading",
    "defaultValue": "Square Banner Configuration"
  },
  {
    "type": "section",
    "capabilities": ["COLOR"],
    "items": [
      {
        "type": "heading",
        "defaultValue": "Banner Colors"
      },
      {
        "type": "text",
        "defaultValue": "Currently, due to technical limitaions, Banner Colors can't be the same, neither of them can be black. The old background color can't be the new foreground color."
      },
      {
        "type": "color",
        "messageKey": "ForegroundColor",
        "defaultValue": "0x00AAFF",
        "label": "Foreground Color"
      },
      {
        "type": "color",
        "messageKey": "BackgroundColor",
        "defaultValue": "0x0055AA",
        "label": "Background Color"
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Bluetooth Disconnect"
      },
      {
        "type": "toggle",
        "messageKey": "DisconnectVibrate",
        "label": "Vibrate on Disconnect",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "DisconnectIcon",
        "label": "Show Icon on Disconnect",
        "defaultValue": true
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];