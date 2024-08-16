# OpenBMC webserver #

This component attempts to be a "do everything" embedded webserver for OpenBMC.


## How to make PRs to this repo for Fortanix specific changes?
- Checkout and pull  the latest ```series3-openbmc``` branch (all the PRs should be merged to this branch)
- Create a dev branch from ```series3-openbmc```
- Once you are done making changes, you should test the changes (at least the build). The way to do this is to push the changes to remote and then take the commit
  hash and the name of the dev branch and update it in ```asrock-openbmc/meta-phosphor/recipes-phosphor/interfaces/bmcweb_git.bb``` file.
  The fields are ```SRC_URI``` (change the ```branch``` field to dev branch name) and ```SRCREV``` to the dev branch commit. 
  (repo link - https://bitbucket.org/fortanix/asrock-openbmc/src/main/). Once you have updated the dev branch name and commit hash, you can build openbmc by
  following the instructions https://bitbucket.org/fortanix/asrock-openbmc/src/main/README.md . 
- Once the build suceeds, raise a PR to ```series3-openbmc``` branch and wait for CI to pass. 
- Once the PR is merged, please raise a PR to update the ```SRCREV``` in ```asrock-openbmc/meta-phosphor/recipes-phosphor/interfaces/bmcweb_git.bb``` with the
  latest commit hash of ```series3-openbmc``` branch of this repo (```bmcweb```). Please  make sure that the ```branch``` field in ```SRC_URI```
  is kept to ```series3-openbmc```.


## Features ##
The webserver implements a few distinct interfaces:
+ DBus event websocket.  Allows registering on changes to specific dbus paths,
  properties, and will send an event from the websocket if those filters match.
+ OpenBMC DBus REST api.  Allows direct, low interference, high fidelity access
  to dbus and the objects it represents.
+ Serial: A serial websocket for interacting with the host serial console
  through websockets.
+ Redfish: A protocol compliant, (Redfish.md)[DBus to Redfish translator].
+ KVM: A websocket based implementation of the RFB (VNC) frame buffer protocol
  intended to mate to webui-vue to provide a complete KVM implementation.

## Protocols ##
bmcweb at a protocol level supports http and https.  TLS is supported through
OpenSSL.

## AuthX ##
### Authentication ###
Bmcweb supports multiple authentication protocols:
+ Basic authentication per RFC7617
+ Cookie based authentication for authenticating against webui-vue
+ Mutual TLS authentication based on OpenSSL
+ Session authentication through webui-vue
+ XToken based authentication conformant to Redfish DSP0266

Each of these types of authentication is able to be enabled or disabled both via
runtime policy changes (through the relevant Redfish APIs) or via configure time
options.  All authentication mechanisms supporting username/password are routed
to libpam, to allow for customization in authentication implementations.

### Authorization ###
All authorization in bmcweb is determined at routing time, and per route, and
conform to the Redfish PrivilegeRegistry.

*Note: Non-Redfish functions are mapped to the closest equivalent Redfish
privilege level.

## Configuration

bmcweb is configured per the
[meson build files](https://mesonbuild.com/Build-options.html).  Available
options are documented in meson_options.txt

## Compile bmcweb with default options:
```ascii
meson builddir
ninja -C builddir
```

If any of the dependencies are not found on the host system during
configuration, meson will automatically download them via its wrap dependencies
mentioned in `bmcweb/subprojects`.

## Debug logging
bmcweb by default is compiled with runtime logging disabled, as a performance
consideration.  To enable it in a standalone build, add the
```ascii
-Dlogging='enabled'
```
option to your configure flags.  If building within Yocto, add the following to your local.conf.
```bash
EXTRA_OEMESON:pn-bmcweb:append = "-Dbmcweb-logging='enabled'"
```

## Use of persistent data
bmcweb relies on some on-system data for storage of persistent data that is
internal to the process.  Details on the exact data stored and when it is
read/written can seen from the persistent_data namespace.

## TLS certificate generation
When SSL support is enabled and a usable certificate is not found, bmcweb will
generate a self-signed a certificate before launching the server.  Please see
the bmcweb source code for details on the parameters this certificate is built
with.

