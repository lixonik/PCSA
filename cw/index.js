import soap from 'soap';
import axios from 'axios';

const SOAP_URL =
  'https://www.dataaccess.com/webservicesserver/NumberConversion.wso?WSDL';
const REST_URL = 'https://jsonplaceholder.typicode.com/todos/1';

async function getSOAPData() {
  try {
    const client = await soap.createClientAsync(SOAP_URL);
    const args = { ubiNum: 42 };
    const result = await client.NumberToWordsAsync(args);
    console.log('SOAP Response:', result[0].NumberToWordsResult);
    console.log('SOAP Response:', result);
  } catch (error) {
    console.error('SOAP Error:', error);
  }
}

async function getRESTData() {
  try {
    const response = await axios.get(REST_URL);
    console.log('REST Response:', response.data);
  } catch (error) {
    console.error('REST Error:', error);
  }
}

async function main() {
  console.log('Fetching data from SOAP service...');
  await getSOAPData();

  console.log('Fetching data from REST service...');
  await getRESTData();
}

main();
