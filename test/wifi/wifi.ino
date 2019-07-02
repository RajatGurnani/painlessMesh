//************************************************************
// this is a simple example that uses the easyMesh library
//
// 1. blinks led once for every node on the mesh
// 2. blink cycle repeats every BLINK_PERIOD
// 3. sends a silly message to every node on the mesh at a random time between 1
// and 5 seconds
// 4. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessmesh/configuration.hpp"

#include "painlessMeshConnection.h"

#include "painlessmesh/mesh.hpp"
#include "painlessmesh/tcp.hpp"

using namespace painlessmesh;
using namespace logger;

painlessmesh::Mesh<MeshConnection> mesh;

std::shared_ptr<AsyncServer> pServer;

void setup() {
  Serial.begin(115200);
  Log.setLogLevel(ERROR | CONNECTION | DEBUG);
  uint8_t mac[] = {0, 0, 0, 0, 0, 0};

  /*auto _apIp = IPAddress(10, (nodeId & 0xFF00) >> 8, (nodeId & 0xFF), 1);
  IPAddress netmask(255, 255, 255, 0);

  WiFi.softAPConfig(_apIp, _apIp, netmask);*/

  WiFi.softAP("otatest", "somethingSneaky", 6);
  if (WiFi.softAPmacAddress(mac) == 0) {
    Log(ERROR, "init(): WiFi.softAPmacAddress(MAC) failed.\n");
  }
  auto nodeId = painlessmesh::tcp::encodeNodeId(mac);
  if (nodeId == 0)
    Log(ERROR, "NodeId set to 0\n");
  Log(ERROR, "Bla %u\n", nodeId);
  mesh.init(nodeId);
  mesh.setRoot(true);

  pServer = std::make_shared<AsyncServer>(5555);
  painlessmesh::tcp::initServer<MeshConnection>((*pServer), mesh);
}

void loop() { mesh.update(); }
