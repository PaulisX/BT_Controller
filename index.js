async function connectToBluetoothDevice() {
   try {
       // Request the Bluetooth device
      const device = await navigator.bluetooth.requestDevice({
         acceptAllDevices: false, // Only allow specific devices
         filters: [
            {  name: 'Tomato_BT',
               services: ["0000ffe0-0000-1000-8000-00805f9b34fb"]
            }
         ]
      })

      console.log('Device found:', device.name);
      console.log('Connecting to device...');

      // Connect to the GATT server
      const server = await device.gatt.connect();
      console.log('Connected to GATT server.');

      // Access a service (e.g., Battery Service)
      const service = await server.getPrimaryService('0000ffe0-0000-1000-8000-00805f9b34fb');
      console.log('Battery service accessed.');

      const characteristic = await service.getCharacteristic('0000ffe1-0000-1000-8000-00805f9b34fb');
      let sliderA = document.getElementById('motA');
      let sliderB = document.getElementById('motB');
      while(true){
         let motA = sliderA.value;
         let motB = sliderB.value;
         const resetEnergyExpended = Int8Array.of(motA,motB);
         await characteristic.writeValue(resetEnergyExpended);
      }      
      

      // // Access a characteristic (e.g., Battery Level)
      // const value = await characteristic.readValue();
      // const batteryLevel = value.getUint8(0);
      // console.log('Battery level:', batteryLevel + '%');
   } catch (error) {
      console.error('Error:', error);
   }
}

// Call the function

// connectToBluetoothDevice();
