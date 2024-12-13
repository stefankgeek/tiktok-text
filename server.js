const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const { WebcastPushConnection } = require('tiktok-live-connector');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

const tiktokUsername = 'votre_nom_utilisateur_tiktok';
const tiktokLiveConnection = new WebcastPushConnection(tiktokUsername);

app.use(express.static('public'));

io.on('connection', (socket) => {
    console.log('Nouveau client connecté');
});

tiktokLiveConnection.connect().then(state => {
    console.log(`Connecté au live de ${tiktokUsername}`);

    tiktokLiveConnection.on('chat', (data) => {
        io.emit('tiktokEvent', { type: 'chat', data });
    });

    tiktokLiveConnection.on('gift', (data) => {
        io.emit('tiktokEvent', { type: 'gift', data });
    });

    tiktokLiveConnection.on('like', (data) => {
        io.emit('tiktokEvent', { type: 'like', data });
    });

    // Ajoutez d'autres événements selon vos besoins

}).catch(err => {
    console.error('Échec de la connexion', err);
});

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
    console.log(`Serveur en écoute sur le port ${PORT}`);
});
