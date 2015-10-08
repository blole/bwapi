#pragma once

#include "SNPNetwork.h"

#include <Util/Types.h>
#include "UDPSocket.h"
#include "SettingsDialog.h"

namespace DRIP
{
  extern SNP::NetworkInfo networkInfo;

  class DirectIP : public SNP::Network<UDPAddr>
  {
    SettingsDialog settingsDialog;
  public:
    void initialize() override;
    void destroy() override;
    void requestAds() override;
    void sendAsyn(const UDPAddr& to, Util::MemoryFrame packet) override;
    void receive() override;
    void startAdvertising(Util::MemoryFrame ad) override;
    void stopAdvertising() override;

    void processIncomingPackets();

  private:
    void rebind();
  };
};
