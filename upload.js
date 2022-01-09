const fs = require('fs');
const path = require('path');
const axios = require('axios');
const FormData = require('form-data')

const config = JSON.parse(fs.readFileSync(path.join(__dirname, 'library.json')));

const file = `build/${config.name}-${config.version}.zip`;
const outputFile = `esp/${config.name}-${config.version}.zip`;

const form = new FormData();
form.append('data', fs.createReadStream(file));
form.append('name', outputFile);

axios({
	url: process.env.UPLOAD_URL || 'http://localhost/upload',
	method: 'post',
	headers: {
		...form.getHeaders()
	},
	auth: {
		username: process.env.USER || 'user',
		password: process.env.PASSWORD || 'password'
	},
	data: form
})
	.then(res => console.log('ok'))
	.catch(err => console.error(err));