const socket = io();
const eventLog = document.getElementById('eventLog');

socket.on('tiktokEvent', (event) => {
    const eventElement = document.createElement('p');
    eventElement.textContent = `${event.type}: ${JSON.stringify(event.data)}`;
    eventLog.appendChild(eventElement);
    eventLog.scrollTop = eventLog.scrollHeight;
});
