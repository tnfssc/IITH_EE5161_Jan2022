# Making a web app to control the UGV

## Prerequisites

- Linux machine
- Node.js (with `corepack enable`-ed)
- Assembled UGV

## Setup

### Clone the repository

- Clone the repository. `git clone https://github.com/tnfssc/IITH_EE5161_Jan2022.git`
- Go into the `IITH_EE5161_Jan2022/codes/esp-ugv-7` directory. `cd IITH_EE5161_Jan2022/codes/esp-ugv-7`

### Flashing the UGV code

- Edit the `src/main.cpp` file and change the WiFi credentials in `Ln 7 and Ln 8`.
- Run the `pio run -t upload` command to upload the code to the UGV.

### Deploy the PWA

- Go into the `pwa` directory. `cd pwa`
- Here are all the static files required for the Web App.
- Use your favourite static site hosting service to deploy the Web App.
- I am going to use [Vercel](https://vercel.com/) for this.
- Install the `vercel` command line tool. `yarn global add vercel`
- Use the `vercel` command to deploy the Web App. `vercel`
- Make a note of the deployment URL (in my case, it is _<https://ugv-controller.vercel.app>_)

### Setup Proxy server

- Go into the `proxy` directory. `cd proxy`
- Install all the dependencies. `yarn install`
- Edit the `index.js` file. Change the URL in `Ln 13, Col 17` to the deployment URL (in my case, it is _<https://ugv-controller.vercel.app>_)
- Edit the `.env` file to set the `ESP_IP` and `ESP_PORT` variables.
- Run the `node index.js` command to start the server.

## Usage

- Open the deployment URL in your browser (Chrome).
- Install the Web App on your phone using the **Add to the Home Screen** button that appears at the bottom.
- This will install the PWA version of the web app on your phone.
- Open the app and enter the address of the proxy server in the **IP Address** field.
- If you used proot-distro ubuntu on termux on your phone, you can leave the default address.
- Use the buttons to send the commands to the UGV.
- Press and hold the **Forward** button to move forward (on mobile you need to tap the button and then hold).
- The web app sends HTTP requests to the proxy server.
- The proxy server sends UDP packets to the UGV based on the HTTP request.

## Potential improvements

- The proxy server can be used to serve the Web App too making it a one stop solution.
- The proxy server can also be provided with an authorized certificate for SSL.
- You could [`bubblewrap`](https://github.com/GoogleChromeLabs/bubblewrap) the PWA into a TWA `.apk` file and install it on your phone or share it.
- Building a `React-Native` app with [`react-native-udp`](https://github.com/tradle/react-native-udp) package could make the whole thing easier for a beginner to use.

## [Code is available here](https://github.com/tnfssc/IITH_EE5161_Jan2022/tree/main/codes/esp-ugv-7)

The code has comments that explain the code.
