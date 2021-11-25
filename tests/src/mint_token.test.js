import { processTest } from "./test.fixture";

const contractName = "PoapDelegatedMint";
// From : https://etherscan.io/tx/0x1c926a7d245bca1384527db34bfd31eb63b4177bb48097504fcbd2f340a05593
const rawTxHex = "0x02f901720109847735940085169321500b83055d07946f2235864cf897078fcdcc2854b76c482cd1687480b901043da5b8f0000000000000000000000000000000000000000000000000000000000000377300000000000000000000000000000000000000000000000000000000002a0990000000000000000000000000f6cf817f2e8a3806fb00b8f882522e46a7fb209100000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000000000041585ed4bb370a426770a45a36d335a1fb6f387c55d9e1bad6e1a1fce9799d7ec80b50c1a0fa59b207e1deecc4b16b04e167f352c3e261110d8eb219ee085d68c61b00000000000000000000000000000000000000000000000000000000000000c080a0d388e05578f65885f6e507657533d10f724a3ee89847f012aa15329fea642b92a018d5ed4e8c7bb86aca58c0691194ba34443125ddec22d78037f857a868243a80";
const testLabel = "Mint Token"; // <= Name of the test
const testDirSuffix = "mint_token"; // <= directory to compare device snapshots to

const devices = [
  {
    name: "nanos",
    label: "Nano S",
    steps: 7, // <= Define the number of steps for this test case and this device
  },
  // {
  //   name: "nanox",
  //   label: "Nano X",
  //   steps: 5, // <= Define the number of steps for this test case and this device
  // },
];

devices.forEach((device) =>
  processTest(device, contractName, testLabel, testDirSuffix, rawTxHex)
);