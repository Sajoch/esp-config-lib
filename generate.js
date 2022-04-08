const fs = require('fs');
const path = require('path');
const content = fs.readFileSync('data_format.json');
const data = JSON.parse(content);

const definedTypes = {
	'string': { cType: 'std::string', getter: 'ReadString' },
	'uint16': { cType: 'uint16_t', getter: 'ReadUint16' },
};

let addressMap = {};
let currentAddress = data.startOffset || 0x20;
const dataVersion = data.version || 1;
const addressIncrement = data.addressIncrement || 2;
const fields = data.fields || [];
const outputFilename = 'Config';

for (const o of fields) {
	addressMap[o.name] = currentAddress;
	currentAddress += addressIncrement;
}

const output = 'build';
fs.promises.mkdir(output, { recursive: true });
fs.writeFileSync(`${output}/${outputFilename}.h`, generateHeader(outputFilename));
fs.writeFileSync(`${output}/${outputFilename}.cpp`, generateSource(outputFilename));

function generateHeader(outputName) {
	let generatedOutput = '';
	generatedOutput += `\t${outputName}(TwoWire* bus, uint8_t address);\n`;
	for (const o of fields) {
		const defined = definedTypes[o.type];
		if (typeof defined === 'undefined')
			continue;

		generatedOutput += `\t${defined.cType} Get${o.name}();\n`;
	}
	return `#pragma once\n#include "ConfigImpl.h"\n\nclass ${outputName} : public ConfigImpl {\npublic:\n${generatedOutput}};\n`;

}

function generateSource(outputName) {
	let generatedOutput = '';
	generatedOutput += `${outputName}::${outputName}(TwoWire* bus, uint8_t address) : ConfigImpl(bus, address, ${dataVersion}) {}\n`;
	for (const o of fields) {
		const defined = definedTypes[o.type];
		if (typeof defined === 'undefined')
			continue;

		const address = addressMap[o.name];

		generatedOutput += `${defined.cType} ${outputName}::Get${o.name}() { return ${defined.getter}(${address}); }\n`;
	}

	return `#include "${outputName}.h"\n\n${generatedOutput}\n`;
}