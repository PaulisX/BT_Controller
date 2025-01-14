let characteristic;
let sliderA = document.getElementById('motA');
let sliderB = document.getElementById('motB');

async function connectToBluetoothDevice() {
   sliderA = document.getElementById('motA');
   sliderB = document.getElementById('motB');
   try {
       // Request the Bluetooth device
      const device = await navigator.bluetooth.requestDevice({
         acceptAllDevices: false, // Only allow specific devices
         filters: [
            {
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

      characteristic = await service.getCharacteristic('0000ffe1-0000-1000-8000-00805f9b34fb');
      const resetEnergyExpended = Int8Array.of(0,0);
      await characteristic.writeValue(resetEnergyExpended);
      await characteristic.writeValue(resetEnergyExpended);
      while(true){
         let motA = parseInt(sliderA.value);
         let motB = parseInt(sliderB.value);

         motA =  Math.abs(motA) << 2 | (motA<0)<<1 | 0;
         motB =  Math.abs(motB) << 2 | (motB<0)<<1 | 1;
         console.log(motA.toString(2), " ; ", motB.toString(2));
         const resetEnergyExpended = Int8Array.of(motA,motB);
         await characteristic.writeValue(resetEnergyExpended); 
         await sleep(50);
      }      
   } catch (error) {
      console.error('Error:', error);
   }
}

function sleep(ms) {
   return new Promise(resolve => setTimeout(resolve, ms));
}
